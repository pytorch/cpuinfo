#if defined(_WIN32) || defined(__CYGWIN__)
	#include <windows.h>
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
	#include <pthread.h>
#endif

#include <inttypes.h>
#include <string.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

#ifdef __APPLE__
	#include "TargetConditionals.h"
#endif


#if defined(_WIN32) || defined(__CYGWIN__)
	static INIT_ONCE init_guard = INIT_ONCE_STATIC_INIT;
#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
	static pthread_once_t init_guard = PTHREAD_ONCE_INIT;
#else
	static bool init_guard = false;
#endif

bool CPUINFO_ABI cpuinfo_initialize(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	#if defined(__MACH__) && defined(__APPLE__)
		pthread_once(&init_guard, &cpuinfo_x86_mach_init);
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
	cpuinfo_is_initialized = false;

	free(cpuinfo_processors);
	free(cpuinfo_cores);
	free(cpuinfo_clusters);
	free(cpuinfo_packages);
	for (size_t cache_level = 0; cache_level < cpuinfo_cache_level_max; cache_level++) {
		free(cpuinfo_cache[cache_level]);
	}
	memset(cpuinfo_cache, 0, cpuinfo_cache_level_max * sizeof(struct cpuinfo_cache*));

	cpuinfo_processors_count = 0;
	cpuinfo_cores_count = 0;
	cpuinfo_clusters_count = 0;
	cpuinfo_packages_count = 0;
	memset(cpuinfo_cache_count, 0, cpuinfo_cache_level_max * sizeof(uint32_t));
	cpuinfo_max_cache_size = 0;

#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64 || CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	free(cpuinfo_uarchs);
	cpuinfo_uarchs_count = 0;
#else
	memset(&cpuinfo_global_uarch, 0, sizeof(struct cpuinfo_uarch_info));
#endif // CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64 || CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64

#ifdef __linux__
	free(cpuinfo_linux_cpu_to_processor_map);
	free(cpuinfo_linux_cpu_to_core_map);
	cpuinfo_linux_cpu_max = 0;
#endif // __linux__
}
