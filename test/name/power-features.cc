#include <cpuinfo.h>
#include <gtest/gtest.h>
#include <sys/auxv.h>

extern "C" bool CPUINFO_ABI cpuinfo_initialize(void);
extern "C" inline bool cpuinfo_has_powerpc_vsx(void);
extern "C" inline bool cpuinfo_has_powerpc_htm(void);
extern "C" inline bool cpuinfo_has_powerpc_mma(void);

#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_1                  UINT32_C(0x00040000)	//To check if the architecture is Power10
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_00                 UINT32_C(0x00800000)    //To check if the architecture is Power9
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_2_07                 UINT32_C(0x80000000)    //To check if the architecture is Power8

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
	#endif  //CPUINFO_MOCK //

	#if (b & CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_1)
		EXPECT_EQ(0, cpuinfo_has_powerpc_htm());
		EXPECT_EQ(1, cpuinfo_has_powerpc_mma());
		EXPECT_EQ(1, cpuinfo_has_powerpc_vsx());
	#elif (b & CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_00)
		EXPECT_EQ(0, cpuinfo_has_powerpc_htm());
		EXPECT_EQ(0, cpuinfo_has_powerpc_mma());
		EXPECT_EQ(1, cpuinfo_has_powerpc_vsx());
	#elif (b & CPUINFO_POWERPC_LINUX_FEATURE_ARCH_2_07)
		EXPECT_EQ(1, cpuinfo_has_powerpc_htm());
		EXPECT_EQ(0, cpuinfo_has_powerpc_mma());
		EXPECT_EQ(1, cpuinfo_has_powerpc_vsx());
	#endif

	cpuinfo_deinitialize();
}
