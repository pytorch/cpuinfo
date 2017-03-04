#include <pthread.h>

#include <cpuinfo.h>
#include <api.h>


struct cpuinfo_processor* cpuinfo_processors;
struct cpuinfo_cores* cpuinfo_cores;
struct cpuinfo_package* cpuinfo_packages;

uint32_t cpuinfo_processors_count;


static pthread_once_t init_guard = PTHREAD_ONCE_INIT;

void CPUINFO_ABI cpuinfo_initialize(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	#if defined(__MACH__) && defined(__APPLE__)
		pthread_once(&init_guard, &cpuinfo_x86_mach_init);
	#elif defined(__linux__)
		pthread_once(&init_guard, &cpuinfo_x86_linux_init);
	#else
		#error Unsupported target OS
	#endif
#else
	#error Unsupported target architecture
#endif
}

void CPUINFO_ABI cpuinfo_deinitialize(void) {
}
