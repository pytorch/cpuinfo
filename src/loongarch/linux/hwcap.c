#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <elf.h>

#if CPUINFO_MOCK
	#include <cpuinfo-mock.h>
#endif
#include <cpuinfo.h>
#include <loongarch/linux/api.h>
#include <cpuinfo/log.h>

#if CPUINFO_ARCH_LOONGARCH64
	#include <sys/auxv.h>
#else
	#define AT_HWCAP 16
#endif


#if CPUINFO_MOCK
	static uint32_t mock_hwcap = 0;
	void cpuinfo_set_hwcap(uint32_t hwcap) {
		mock_hwcap = hwcap;
	}
#endif


#if CPUINFO_ARCH_LOONGARCH64
	void cpuinfo_loongarch_linux_hwcap_from_getauxval(
		uint32_t hwcap[restrict static 1]
	)
	{
		#if CPUINFO_MOCK
			*hwcap  = mock_hwcap;
		#else
			*hwcap  = (uint32_t) getauxval(AT_HWCAP);
			return ;
		#endif
	}
#endif
