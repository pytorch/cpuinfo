#include <gtest/gtest.h>

#include <cpuinfo.h>


TEST(CURRENT_PROCESSOR, not_null) {
	ASSERT_TRUE(cpuinfo_initialize());

	ASSERT_TRUE(cpuinfo_get_current_processor());
}

TEST(CURRENT_PROCESSOR, within_bounds) {
	ASSERT_TRUE(cpuinfo_initialize());

	const struct cpuinfo_processor* current_processor = cpuinfo_get_current_processor();
	const struct cpuinfo_processor* processors_begin = cpuinfo_get_processors();
	const struct cpuinfo_processor* processors_end = processors_begin + cpuinfo_get_processors_count();
	ASSERT_GE(current_processor, processors_begin);
	ASSERT_LT(current_processor, processors_end);
}

TEST(CURRENT_CORE, not_null) {
	ASSERT_TRUE(cpuinfo_initialize());

	ASSERT_TRUE(cpuinfo_get_current_core());
}

TEST(CURRENT_CORE, within_bounds) {
	ASSERT_TRUE(cpuinfo_initialize());

	const struct cpuinfo_core* current_core = cpuinfo_get_current_core();
	const struct cpuinfo_core* cores_begin = cpuinfo_get_cores();
	const struct cpuinfo_core* cores_end = cores_begin + cpuinfo_get_cores_count();
	ASSERT_GE(current_core, cores_begin);
	ASSERT_LT(current_core, cores_end);
}
