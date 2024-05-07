#include <cpuinfo.h>
#include <gtest/gtest.h>
#include <sys/auxv.h>

extern "C" bool CPUINFO_ABI cpuinfo_initialize(void);
extern "C" inline bool cpuinfo_has_powerpc_vsx(void);
extern "C" inline bool cpuinfo_has_powerpc_htm(void);
extern "C" inline bool cpuinfo_has_powerpc_mma(void);

TEST(PowerFeatures, Power){
	if (!cpuinfo_initialize()) {
		fprintf(stderr, "failed to initialize CPU information\n");
		exit(1);
	}
	#if CPUINFO_MOCK
		int a = mock_hwcap;
	#else
		int a = (uint32_t) getauxval(AT_HWCAP);
		int b = (uint32_t) getauxval(AT_HWCAP2);
	#endif

	EXPECT_EQ(!!(b & UINT32_C(0x40000000)), cpuinfo_has_powerpc_htm());
	EXPECT_EQ(!!(b & UINT32_C(0x00020000)), cpuinfo_has_powerpc_mma());
	EXPECT_EQ(!!(a & UINT32_C(0x00000080)), cpuinfo_has_powerpc_vsx());

	cpuinfo_deinitialize();
}
