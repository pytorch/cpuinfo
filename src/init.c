#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
#include <pthread.h>
#endif

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
static INIT_ONCE init_guard = INIT_ONCE_STATIC_INIT;
static INIT_ONCE deinit_guard = INIT_ONCE_STATIC_INIT;
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
static pthread_once_t init_guard = PTHREAD_ONCE_INIT;
static pthread_once_t deinit_guard = PTHREAD_ONCE_INIT;
#else
static bool init_guard = false;
static bool deinit_guard = false;
#endif

bool CPUINFO_ABI cpuinfo_initialize(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
#if defined(__MACH__) && defined(__APPLE__)
	pthread_once(&init_guard, &cpuinfo_x86_mach_init);
#elif defined(__FreeBSD__)
	pthread_once(&init_guard, &cpuinfo_x86_freebsd_init);
#elif defined(__linux__)
	pthread_once(&init_guard, &cpuinfo_x86_linux_init);
#elif defined(_WIN32) || defined(__CYGWIN__)
	InitOnceExecuteOnce(&init_guard, &cpuinfo_x86_windows_init, NULL, NULL);
#else
	cpuinfo_log_error("operating system is not supported in cpuinfo");
#endif
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
#if defined(__linux__)
	pthread_once(&init_guard, &cpuinfo_arm_linux_init);
#elif defined(__MACH__) && defined(__APPLE__)
	pthread_once(&init_guard, &cpuinfo_arm_mach_init);
#elif defined(_WIN32)
	InitOnceExecuteOnce(&init_guard, &cpuinfo_arm_windows_init, NULL, NULL);
#else
	cpuinfo_log_error("operating system is not supported in cpuinfo");
#endif
#elif CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
#if defined(__linux__)
	pthread_once(&init_guard, &cpuinfo_riscv_linux_init);
#else
	cpuinfo_log_error("operating system is not supported in cpuinfo");
#endif
#elif CPUINFO_ARCH_ASMJS || CPUINFO_ARCH_WASM || CPUINFO_ARCH_WASMSIMD
#if defined(__EMSCRIPTEN_PTHREADS__)
	pthread_once(&init_guard, &cpuinfo_emscripten_init);
#else
	if (!init_guard) {
		cpuinfo_emscripten_init();
	}
	init_guard = true;
#endif
#else
	cpuinfo_log_error("processor architecture is not supported in cpuinfo");
#endif
	return cpuinfo_is_initialized;
}

void CPUINFO_ABI cpuinfo_deinitialize(void) {
	if (!cpuinfo_is_initialized) {
		return;
	}
	cpuinfo_is_initialized = false;
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
#if defined(__MACH__) && defined(__APPLE__)
	pthread_once(&deinit_guard, &cpuinfo_x86_mach_deinit);
#elif defined(__FreeBSD__)
	pthread_once(&deinit_guard, &cpuinfo_x86_freebsd_deinit);
#elif defined(__linux__)
	pthread_once(&deinit_guard, &cpuinfo_x86_linux_deinit);
#elif defined(_WIN32) || defined(__CYGWIN__)
	InitOnceExecuteOnce(&deinit_guard, &cpuinfo_x86_windows_deinit, NULL, NULL);
#endif
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
#if defined(__linux__)
	pthread_once(&deinit_guard, &cpuinfo_arm_linux_deinit);
#elif defined(__MACH__) && defined(__APPLE__)
	pthread_once(&deinit_guard, &cpuinfo_arm_mach_deinit);
#elif defined(_WIN32)
	InitOnceExecuteOnce(&deinit_guard, &cpuinfo_arm_windows_deinit, NULL, NULL);
#endif
#elif CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
#if defined(__linux__)
	pthread_once(&deinit_guard, &cpuinfo_riscv_linux_deinit);
#endif
#elif CPUINFO_ARCH_ASMJS || CPUINFO_ARCH_WASM || CPUINFO_ARCH_WASMSIMD
#if defined(__EMSCRIPTEN_PTHREADS__)
	pthread_once(&deinit_guard, &cpuinfo_emscripten_deinit);
#else
	if (!deinit_guard) {
		cpuinfo_emscripten_deinit();
	}
	deinit_guard = true;
#endif
#endif
	/* Reset guards to allow re-initialization */
#if defined(_WIN32) || defined(__CYGWIN__)
	init_guard = (INIT_ONCE)INIT_ONCE_STATIC_INIT;
	deinit_guard = (INIT_ONCE)INIT_ONCE_STATIC_INIT;
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
	init_guard = (pthread_once_t)PTHREAD_ONCE_INIT;
	deinit_guard = (pthread_once_t)PTHREAD_ONCE_INIT;
#else
	init_guard = false;
	deinit_guard = false;
#endif
}
