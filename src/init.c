#include <pthread.h>

#include <cpuinfo.h>
#include <api.h>


struct cpuinfo_processor* cpuinfo_processors;
struct cpuinfo_cores* cpuinfo_cores;
struct cpuinfo_package* cpuinfo_packages;
struct cpuinfo_cache* cpuinfo_l1i_cache;
struct cpuinfo_cache* cpuinfo_l1d_cache;
struct cpuinfo_cache* cpuinfo_l2_cache;
struct cpuinfo_cache* cpuinfo_l3_cache;
struct cpuinfo_cache* cpuinfo_l4_cache;


static pthread_once_t init_guard = PTHREAD_ONCE_INIT;

void CPUINFO_ABI cpuinfo_initialize(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	pthread_once(&init_guard, &cpuinfo_x86_mach_init);
#else
	#error Unsupported target architecture
#endif
}

void CPUINFO_ABI cpuinfo_deinitialize(void) {
}
