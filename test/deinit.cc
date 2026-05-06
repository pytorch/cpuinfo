#include <gtest/gtest.h>

#include <cpuinfo.h>

TEST(DEINITIALIZE, clears_processors) {
	ASSERT_TRUE(cpuinfo_initialize());
	EXPECT_NE(0, cpuinfo_get_processors_count());
	EXPECT_TRUE(cpuinfo_get_processors());

	cpuinfo_deinitialize();

	EXPECT_EQ(0, cpuinfo_get_processors_count());
	EXPECT_FALSE(cpuinfo_get_processors());
}

TEST(DEINITIALIZE, clears_cores) {
	ASSERT_TRUE(cpuinfo_initialize());
	EXPECT_NE(0, cpuinfo_get_cores_count());
	EXPECT_TRUE(cpuinfo_get_cores());

	cpuinfo_deinitialize();

	EXPECT_EQ(0, cpuinfo_get_cores_count());
	EXPECT_FALSE(cpuinfo_get_cores());
}

TEST(DEINITIALIZE, clears_clusters) {
	ASSERT_TRUE(cpuinfo_initialize());
	EXPECT_NE(0, cpuinfo_get_clusters_count());
	EXPECT_TRUE(cpuinfo_get_clusters());

	cpuinfo_deinitialize();

	EXPECT_EQ(0, cpuinfo_get_clusters_count());
	EXPECT_FALSE(cpuinfo_get_clusters());
}

TEST(DEINITIALIZE, clears_packages) {
	ASSERT_TRUE(cpuinfo_initialize());
	EXPECT_NE(0, cpuinfo_get_packages_count());
	EXPECT_TRUE(cpuinfo_get_packages());

	cpuinfo_deinitialize();

	EXPECT_EQ(0, cpuinfo_get_packages_count());
	EXPECT_FALSE(cpuinfo_get_packages());
}

TEST(DEINITIALIZE, clears_caches) {
	ASSERT_TRUE(cpuinfo_initialize());

	cpuinfo_deinitialize();

	EXPECT_EQ(0, cpuinfo_get_l1i_caches_count());
	EXPECT_EQ(0, cpuinfo_get_l1d_caches_count());
	EXPECT_EQ(0, cpuinfo_get_l2_caches_count());
	EXPECT_EQ(0, cpuinfo_get_l3_caches_count());
	EXPECT_EQ(0, cpuinfo_get_l4_caches_count());
	EXPECT_FALSE(cpuinfo_get_l1i_caches());
	EXPECT_FALSE(cpuinfo_get_l1d_caches());
	EXPECT_FALSE(cpuinfo_get_l2_caches());
	EXPECT_FALSE(cpuinfo_get_l3_caches());
	EXPECT_FALSE(cpuinfo_get_l4_caches());
}

TEST(DEINITIALIZE, clears_uarchs) {
	ASSERT_TRUE(cpuinfo_initialize());
	EXPECT_NE(0, cpuinfo_get_uarchs_count());
	EXPECT_TRUE(cpuinfo_get_uarchs());

	cpuinfo_deinitialize();

	EXPECT_EQ(0, cpuinfo_get_uarchs_count());
	EXPECT_FALSE(cpuinfo_get_uarchs());
}

TEST(DEINITIALIZE, double_deinit_is_safe) {
	ASSERT_TRUE(cpuinfo_initialize());
	cpuinfo_deinitialize();
	cpuinfo_deinitialize();
	EXPECT_EQ(0, cpuinfo_get_processors_count());
}

TEST(DEINITIALIZE, before_init_is_safe) {
	cpuinfo_deinitialize();
	EXPECT_EQ(0, cpuinfo_get_processors_count());
}
