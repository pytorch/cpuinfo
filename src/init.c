#ifdef _WIN32
	#include <windows.h>
#else
	#include <pthread.h>
#endif

#include <cpuinfo.h>
#include <api.h>

#ifdef __APPLE__
	#include "TargetConditionals.h"
#endif


struct cpuinfo_processor* cpuinfo_processors = NULL;
struct cpuinfo_core* cpuinfo_cores = NULL;
struct cpuinfo_package* cpuinfo_packages = NULL;

uint32_t cpuinfo_processors_count = 0;
uint32_t cpuinfo_cores_count = 0;
uint32_t cpuinfo_packages_count = 0;


#ifdef _WIN32
	static INIT_ONCE init_guard = INIT_ONCE_STATIC_INIT;
#else
	static pthread_once_t init_guard = PTHREAD_ONCE_INIT;
#endif

void CPUINFO_ABI cpuinfo_initialize(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	#if defined(__MACH__) && defined(__APPLE__)
		pthread_once(&init_guard, &cpuinfo_x86_mach_init);
	#elif defined(__linux__)
		pthread_once(&init_guard, &cpuinfo_x86_linux_init);
	#elif defined(_WIN32)
		InitOnceExecuteOnce(&init_guard, &cpuinfo_x86_windows_init, NULL, NULL);
	#else
		#error Unsupported target OS
	#endif
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	#if defined(__linux__)
		pthread_once(&init_guard, &cpuinfo_arm_linux_init);
	#elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
		pthread_once(&init_guard, &cpuinfo_arm_mach_init);
	#else
		#error Unsupported target OS
	#endif
#else
	#error Unsupported target architecture
#endif
}

void CPUINFO_ABI cpuinfo_deinitialize(void) {
}
