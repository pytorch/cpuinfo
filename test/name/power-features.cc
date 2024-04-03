#include <string.h>
#include <cpuinfo.h>
#include <gtest/gtest.h>

TEST(PowerFeatures, Power){
	if (!cpuinfo_initialize()) {
		fprintf(stderr, "failed to initialize CPU information\n");
		exit(1);
	}
	EXPECT_EQ(1, cpuinfo_has_powerpc_htm());
	EXPECT_EQ(1, cpuinfo_has_powerpc_mma());
	EXPECT_EQ(1, cpuinfo_has_powerpc_vsx());

	cpuinfo_deinitialize();

}
