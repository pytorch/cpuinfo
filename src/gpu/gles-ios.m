#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>

#import <cpuinfo.h>
#import <log.h>
#import <gpu/api.h>


#if ! __has_feature(objc_arc)
	#error "This file must be built with Automatic Reference Counting (-fobjc-arc option)"
#endif

void cpuinfo_gpu_ios_query_gles2(char gpu_name[restrict static CPUINFO_GPU_NAME_MAX]) {
	EAGLContext* existing_context = [EAGLContext currentContext];
	EAGLContext* current_context = nil;
	if (existing_context == nil) {
		/* No existing context: create new context */

		/* OpenGL ES 2.0 is supported by iPhone 3GS and up */
		current_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		if (!current_context) {
			cpuinfo_log_warning("failed to initialize OpenGLES context");
		}

		/* Set context */
		if (![EAGLContext setCurrentContext: current_context]) {
			cpuinfo_log_warning("failed to set current OpenGLES context");
		}
	}
	
	const char* renderer_str = (const char*) glGetString(GL_RENDERER);
	if (renderer_str != NULL) {
		strncpy(gpu_name, renderer_str, CPUINFO_GPU_NAME_MAX - 1);
		gpu_name[CPUINFO_GPU_NAME_MAX - 1] = '\0';
		cpuinfo_log_debug("GL_RENDERER: %s", renderer_str);
	} else {
		cpuinfo_log_warning("failed to get GL_RENDERER for OpenGL ES2 context");
	}

	if (current_context) {
		/* Reset context */
		if (![EAGLContext setCurrentContext: nil]) {
			cpuinfo_log_warning("failed to reset OpenGLES context");
		}
		current_context = nil;
	}
}
