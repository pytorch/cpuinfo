#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include <dlfcn.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <api.h>
#include <log.h>


typedef EGLint (EGLAPIENTRY *egl_get_error_t)(void);
typedef EGLDisplay (EGLAPIENTRY *egl_get_display_t)(EGLNativeDisplayType);
typedef EGLBoolean (EGLAPIENTRY *egl_initialize_t)(EGLDisplay, EGLint*, EGLint*);
typedef EGLBoolean (EGLAPIENTRY *egl_terminate_t)(EGLDisplay);
typedef EGLBoolean (EGLAPIENTRY *egl_choose_config_t)(EGLDisplay dpy, const EGLint*, EGLConfig*, EGLint, EGLint*);
typedef EGLSurface (EGLAPIENTRY *egl_create_pbuffer_surface_t)(EGLDisplay, EGLConfig, const EGLint*);
typedef EGLBoolean (EGLAPIENTRY *egl_destroy_surface_t)(EGLDisplay, EGLSurface);
typedef EGLContext (EGLAPIENTRY *egl_create_context_t)(EGLDisplay, EGLConfig, EGLContext, const EGLint*);
typedef EGLBoolean (EGLAPIENTRY *egl_destroy_context_t)(EGLDisplay, EGLContext);
typedef EGLBoolean (EGLAPIENTRY *egl_make_current_t)(EGLDisplay, EGLSurface, EGLSurface, EGLContext);
typedef EGLContext (EGLAPIENTRY *egl_get_current_context_t)(void);
typedef EGLSurface (EGLAPIENTRY *egl_get_current_surface_t)(EGLint);

static const char egl_function_names[] =
	"eglGetError\0"
	"eglGetDisplay\0"
	"eglInitialize\0"
	"eglTerminate\0"
	"eglChooseConfig\0"
	"eglCreatePbufferSurface\0"
	"eglDestroySurface\0"
	"eglCreateContext\0"
	"eglDestroyContext\0"
	"eglMakeCurrent\0"
	"eglGetCurrentContext\0"
	"eglGetCurrentSurface\0";

#define LIBEGL_FUNCTION_COUNT 12

struct libegl {
	void* handle;
	union {
		struct {
			egl_get_error_t get_error;
			egl_get_display_t get_display;
			egl_initialize_t initialize;
			egl_terminate_t terminate;
			egl_choose_config_t choose_config;
			egl_create_pbuffer_surface_t create_pbuffer_surface;
			egl_destroy_surface_t destroy_surface;
			egl_create_context_t create_context;
			egl_destroy_context_t destroy_context;
			egl_make_current_t make_current;
			egl_get_current_context_t get_current_context;
			egl_get_current_surface_t get_current_surface;
		};
		void* functions[LIBEGL_FUNCTION_COUNT];		
	};
};

typedef const GLubyte* (GL_APIENTRY *gl_get_string_t)(GLenum);

static const char gles2_function_names[] =
	"glGetString\0";

#define LIBGLES2_FUNCTION_COUNT 1

struct libgles2 {
	void* handle;
	union {
		gl_get_string_t get_string;
		void* functions[LIBGLES2_FUNCTION_COUNT];		
	};
};

static bool cpuinfo_gpu_load_libegl(struct libegl egl[restrict static 1]) {
	egl->handle = dlopen("libEGL.so", RTLD_LAZY | RTLD_LOCAL);
	if (egl->handle == NULL) {
		cpuinfo_log_warning("failed to load libEGL.so: %s", dlerror());
		goto failed;
	}

	const char* function_name = egl_function_names;
	for (size_t i = 0; i < LIBEGL_FUNCTION_COUNT; i++) {
		void* function = dlsym(egl->handle, function_name);
		if (function == NULL) {
			cpuinfo_log_warning("failed to locate %s in libEGL.so: %s", function_name, dlerror());
			goto failed;
		}
		egl->functions[i] = function;
		function_name += strlen(function_name) + 1;
	}

	return true;

failed:
	if (egl->handle != NULL) {
		if (dlclose(egl->handle) != 0) {
			cpuinfo_log_info("failed to unload libEGL.so: %s", dlerror());
		}
	}
	memset(egl, 0, sizeof(struct libegl));
	return false;
}

static bool cpuinfo_gpu_load_gles2(struct libgles2 gles[restrict static 1]) {
	gles->handle = dlopen("libGLESv2.so", RTLD_LAZY | RTLD_LOCAL);
	if (gles->handle == NULL) {
		cpuinfo_log_warning("failed to load libGLESv2.so: %s", dlerror());
		goto failed;
	}

	const char* function_name = gles2_function_names;
	for (size_t i = 0; i < LIBGLES2_FUNCTION_COUNT; i++) {
		void* function = dlsym(gles->handle, function_name);
		if (function == NULL) {
			cpuinfo_log_warning("failed to locate %s in libGLESv2.so: %s", function_name, dlerror());
			goto failed;
		}
		gles->functions[i] = function;
		function_name += strlen(function_name) + 1;
	}

	return true;

failed:
	if (gles->handle != NULL) {
		if (dlclose(gles->handle) != 0) {
			cpuinfo_log_info("failed to unload libGLESv2.so: %s", dlerror());
		}
	}
	memset(gles, 0, sizeof(struct libgles2));
	return false;
}

bool cpuinfo_gpu_query_gles2(char gpu_name[restrict static CPUINFO_GPU_NAME_MAX]) {
	bool success = false;
	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLSurface previous_draw_surface = EGL_NO_SURFACE;
	EGLSurface previous_read_surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
	EGLContext previous_context = EGL_NO_CONTEXT;
	EGLBoolean egl_init_success = EGL_FALSE;
	EGLBoolean egl_make_current_success = EGL_FALSE;
	EGLBoolean egl_success;
	struct libegl egl;
	struct libgles2 gles2;

	if (!cpuinfo_gpu_load_libegl(&egl)) {
		return false;
	}
	if (!cpuinfo_gpu_load_gles2(&gles2)) {
		goto cleanup;
	}

	display = egl.get_display(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY) {
		cpuinfo_log_warning("failed to get default EGL display: error %"PRId32, (int32_t) egl.get_error());
		goto cleanup;
	}

	EGLint egl_major = 0, egl_minor = 0;
	egl_init_success = egl.initialize(display, &egl_major, &egl_minor);
	if (egl_init_success != EGL_TRUE) {
		cpuinfo_log_warning("failed to initialize EGL display connection: error %"PRId32, (int32_t) egl.get_error());
		goto cleanup;
	}
	cpuinfo_log_debug("initialized EGL %"PRId32".%"PRId32" display connection",
		(int32_t) egl_major, (int32_t) egl_minor);

	/* Get previous state to restore it later */
	previous_context = egl.get_current_context();
	previous_draw_surface = egl.get_current_surface(EGL_DRAW);
	previous_read_surface = egl.get_current_surface(EGL_READ);

	EGLint const config_attributes[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_NONE,
	};
	EGLConfig config = NULL;
	EGLint config_count = 0;
	egl_success = egl.choose_config(display, config_attributes, &config, 1, &config_count);
	if (egl_success != EGL_TRUE || config_count == 0 || config == NULL) {
		cpuinfo_log_warning("failed to find EGL framebuffer configuration with required attributes");
		goto cleanup;
	}

	EGLint const surface_attributes[] = {
		EGL_HEIGHT, 1,
		EGL_WIDTH, 1,
		EGL_NONE,
	};
	surface = egl.create_pbuffer_surface(display, config, surface_attributes);
	if (surface == EGL_NO_SURFACE) {
		cpuinfo_log_warning("failed to create EGL PBuffer surface: error %"PRId32, (int32_t) egl.get_error());
		goto cleanup;
	}

	EGLint const context_attributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE,
	};
	context = egl.create_context(display, config, EGL_NO_CONTEXT, context_attributes);
	if (context == EGL_NO_CONTEXT) {
		cpuinfo_log_warning("failed to create OpenGL ES2 context: error %"PRId32, (int32_t) egl.get_error());
		goto cleanup;
	}

	egl_make_current_success = egl.make_current(display, surface, surface, context);
	if (egl_make_current_success != EGL_TRUE) {
		cpuinfo_log_warning("failed to attach OpenGL ES rendering context: error %"PRId32, (int32_t) egl.get_error());
		goto cleanup;
	}

	const char* renderer = (const char*) gles2.get_string(GL_RENDERER);
	if (renderer == NULL) {
		cpuinfo_log_warning("failed to get GL_RENDERER for OpenGL ES2 context");
		goto cleanup;
	}
	cpuinfo_log_debug("GL_RENDERER = \"%s\"", renderer);

	strncpy(gpu_name, renderer, CPUINFO_GPU_NAME_MAX);
	success = true;

cleanup:
	if (egl_make_current_success == EGL_TRUE) {
		egl.make_current(display, previous_draw_surface, previous_read_surface, previous_context);
	}
	if (context != EGL_NO_CONTEXT) {
		egl.destroy_context(display, context);
	}
	if (surface != EGL_NO_SURFACE) {
		egl.destroy_surface(display, surface);
	}
	if (egl_init_success == EGL_TRUE) {
		egl.terminate(display);
	}
	if (egl.handle != NULL) {
		dlclose(egl.handle);
	}
	if (gles2.handle != NULL) {
		dlclose(gles2.handle);
	}
	return success;
}
