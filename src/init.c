#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
#include <pthread.h>
#include <stdlib.h>
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

/*
 * Cleanup support for platforms where cpuinfo may be dynamically loaded/unloaded.
 * ARM Linux and Emscripten are excluded because they use static storage for some
 * globals (packages, L3 cache) that cannot be safely freed.
 */
#if defined(_WIN32) || defined(__CYGWIN__)
#define CPUINFO_FREE(ptr)                                            \
	do {                                                         \
		if (ptr)                                             \
			HeapFree(GetProcessHeap(), 0, (void*)(ptr)); \
	} while (0)
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#define CPUINFO_FREE(ptr) free(ptr)
#endif

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)

static void cpuinfo_cleanup(void) {
	if (!cpuinfo_is_initialized) {
		return;
	}

	cpuinfo_is_initialized = false;

	CPUINFO_FREE(cpuinfo_processors);
	cpuinfo_processors = NULL;
	cpuinfo_processors_count = 0;

	CPUINFO_FREE(cpuinfo_cores);
	cpuinfo_cores = NULL;
	cpuinfo_cores_count = 0;

	CPUINFO_FREE(cpuinfo_clusters);
	cpuinfo_clusters = NULL;
	cpuinfo_clusters_count = 0;

	CPUINFO_FREE(cpuinfo_packages);
	cpuinfo_packages = NULL;
	cpuinfo_packages_count = 0;

	/* ARM64 Windows allocates all cache levels as a single contiguous block.
	 * All other supported platforms allocate each level separately. */
#if CPUINFO_ARCH_ARM64 && defined(_WIN32)
	CPUINFO_FREE(cpuinfo_cache[cpuinfo_cache_level_1i]);
#else
	for (int lvl = 0; lvl < cpuinfo_cache_level_max; ++lvl) {
		CPUINFO_FREE(cpuinfo_cache[lvl]);
	}
#endif
	for (int lvl = 0; lvl < cpuinfo_cache_level_max; ++lvl) {
		cpuinfo_cache[lvl] = NULL;
		cpuinfo_cache_count[lvl] = 0;
	}
	cpuinfo_max_cache_size = 0;

#if (CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64 || CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64)
	CPUINFO_FREE(cpuinfo_uarchs);
	cpuinfo_uarchs = NULL;
	cpuinfo_uarchs_count = 0;
#endif

#ifdef __linux__
	CPUINFO_FREE(cpuinfo_linux_cpu_to_processor_map);
	cpuinfo_linux_cpu_to_processor_map = NULL;

	CPUINFO_FREE(cpuinfo_linux_cpu_to_core_map);
	cpuinfo_linux_cpu_to_core_map = NULL;

#if (CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64 || CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64)
	CPUINFO_FREE(cpuinfo_linux_cpu_to_uarch_index_map);
	cpuinfo_linux_cpu_to_uarch_index_map = NULL;
#endif

	cpuinfo_linux_cpu_max = 0;
#endif
}

#endif

#if defined(_WIN32) || defined(__CYGWIN__)
static BOOL CALLBACK cpuinfo_deinit_once(PINIT_ONCE once, PVOID param, PVOID* ctx) {
	cpuinfo_cleanup();
	return TRUE;
}
#endif

void CPUINFO_ABI cpuinfo_deinitialize(void) {
#if defined(_WIN32) || defined(__CYGWIN__)
	InitOnceExecuteOnce(&deinit_guard, cpuinfo_deinit_once, NULL, NULL);
#elif (defined(__linux__) && !CPUINFO_ARCH_ARM) || defined(__APPLE__) || defined(__FreeBSD__)
	pthread_once(&deinit_guard, cpuinfo_cleanup);
#endif
}
