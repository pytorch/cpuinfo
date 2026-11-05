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
static SRWLOCK cpuinfo_lifecycle_lock = SRWLOCK_INIT;
#define CPUINFO_LIFECYCLE_LOCK() AcquireSRWLockExclusive(&cpuinfo_lifecycle_lock)
#define CPUINFO_LIFECYCLE_UNLOCK() ReleaseSRWLockExclusive(&cpuinfo_lifecycle_lock)
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
static pthread_mutex_t cpuinfo_lifecycle_lock = PTHREAD_MUTEX_INITIALIZER;
#define CPUINFO_LIFECYCLE_LOCK() pthread_mutex_lock(&cpuinfo_lifecycle_lock)
#define CPUINFO_LIFECYCLE_UNLOCK() pthread_mutex_unlock(&cpuinfo_lifecycle_lock)
#else
/* Single-threaded build (e.g. Emscripten without pthreads): no lock needed. */
#define CPUINFO_LIFECYCLE_LOCK() \
	do {                     \
	} while (0)
#define CPUINFO_LIFECYCLE_UNLOCK() \
	do {                       \
	} while (0)
#endif
static unsigned int cpuinfo_init_refcount = 0;

bool CPUINFO_ABI cpuinfo_initialize(void) {
	CPUINFO_LIFECYCLE_LOCK();
	if (cpuinfo_init_refcount == 0) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
#if defined(__MACH__) && defined(__APPLE__)
		cpuinfo_x86_mach_init();
#elif defined(__FreeBSD__)
		cpuinfo_x86_freebsd_init();
#elif defined(__linux__)
		cpuinfo_x86_linux_init();
#elif defined(_WIN32) || defined(__CYGWIN__)
		cpuinfo_x86_windows_init(NULL, NULL, NULL);
#else
		cpuinfo_log_error("operating system is not supported in cpuinfo");
#endif
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
#if defined(__linux__)
		cpuinfo_arm_linux_init();
#elif defined(__MACH__) && defined(__APPLE__)
		cpuinfo_arm_mach_init();
#elif defined(_WIN32)
		cpuinfo_arm_windows_init(NULL, NULL, NULL);
#else
		cpuinfo_log_error("operating system is not supported in cpuinfo");
#endif
#elif CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
#if defined(__linux__)
		cpuinfo_riscv_linux_init();
#else
		cpuinfo_log_error("operating system is not supported in cpuinfo");
#endif
#elif CPUINFO_ARCH_ASMJS || CPUINFO_ARCH_WASM || CPUINFO_ARCH_WASMSIMD
		cpuinfo_emscripten_init();
#else
		cpuinfo_log_error("processor architecture is not supported in cpuinfo");
#endif
	}
	if (cpuinfo_is_initialized) {
		cpuinfo_init_refcount++;
	}
	const bool initialized = cpuinfo_is_initialized;
	CPUINFO_LIFECYCLE_UNLOCK();
	return initialized;
}

void CPUINFO_ABI cpuinfo_deinitialize(void) {
	CPUINFO_LIFECYCLE_LOCK();
	if (cpuinfo_init_refcount != 0 && --cpuinfo_init_refcount == 0) {
		cpuinfo_is_initialized = false;
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
#if defined(__MACH__) && defined(__APPLE__)
		cpuinfo_x86_mach_deinit();
#elif defined(__FreeBSD__)
		cpuinfo_x86_freebsd_deinit();
#elif defined(__linux__)
		cpuinfo_x86_linux_deinit();
#elif defined(_WIN32) || defined(__CYGWIN__)
		cpuinfo_x86_windows_deinit(NULL, NULL, NULL);
#endif
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
#if defined(__linux__)
		cpuinfo_arm_linux_deinit();
#elif defined(__MACH__) && defined(__APPLE__)
		cpuinfo_arm_mach_deinit();
#elif defined(_WIN32)
		cpuinfo_arm_windows_deinit(NULL, NULL, NULL);
#endif
#elif CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
#if defined(__linux__)
		cpuinfo_riscv_linux_deinit();
#endif
#elif CPUINFO_ARCH_ASMJS || CPUINFO_ARCH_WASM || CPUINFO_ARCH_WASMSIMD
		cpuinfo_emscripten_deinit();
#endif
	}
	CPUINFO_LIFECYCLE_UNLOCK();
}
