#include <riscv/linux/api.h>

#if CPUINFO_MOCK
	#include <cpuinfo-mock.h>

	static uint32_t mock_hwcap = 0;
	void cpuinfo_set_hwcap(uint32_t hwcap) {
		mock_hwcap = hwcap;
	}
#endif

#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	#include <sys/auxv.h>

	void cpuinfo_riscv_linux_hwcap_from_getauxval(
		uint32_t hwcap[restrict static 1])
	{
		#if CPUINFO_MOCK
			*hwcap  = mock_hwcap;
		#else
			*hwcap  = (uint32_t) getauxval(AT_HWCAP);
			return ;
		#endif
	}
#endif