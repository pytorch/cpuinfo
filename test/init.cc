#include <gtest/gtest.h>

#include <cpuinfo.h>


TEST(PROCESSORS_COUNT, non_zero) {
	ASSERT_NE(0, cpuinfo_processors_count);
}

TEST(PROCESSORS, non_null) {
	ASSERT_TRUE(cpuinfo_processors);
}

TEST(CORES_COUNT, non_zero) {
	ASSERT_NE(0, cpuinfo_cores_count);
}

TEST(CORES, non_null) {
	ASSERT_TRUE(cpuinfo_cores);
}

TEST(CORES, known_vendor) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_NE(cpuinfo_vendor_unknown, cpuinfo_cores[i].vendor);
	}
}

TEST(CORES, known_uarch) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_NE(cpuinfo_uarch_unknown, cpuinfo_cores[i].uarch);
	}
}

TEST(L1I, non_zero_count) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_NE(0, l1i.count);
}

TEST(L1I, valid_count) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_LE(l1i.count, cpuinfo_processors_count);
}

TEST(L1I, non_null) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_TRUE(l1i.instances);
}

TEST(L1I, non_zero_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_NE(0, l1i.instances[k].size);
	}
}

TEST(L1I, valid_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(l1i.instances[k].size,
			l1i.instances[k].associativity * l1i.instances[k].sets * l1i.instances[k].partitions * l1i.instances[k].line_size);
	}
}

TEST(L1I, non_zero_associativity) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_NE(0, l1i.instances[k].associativity);
	}
}

TEST(L1I, non_zero_partitions) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_NE(0, l1i.instances[k].partitions);
	}
}

TEST(L1I, non_zero_line_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_NE(0, l1i.instances[k].line_size);
	}
}

TEST(L1I, power_of_2_line_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(0, l1i.instances[k].line_size & (l1i.instances[k].line_size - 1));
	}
}

TEST(L1I, valid_line_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_GE(l1i.instances[k].line_size, 16);
		ASSERT_LE(l1i.instances[k].line_size, 128);
	}
}

TEST(L1I, valid_flags) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(0, l1i.instances[k].flags & ~valid_flags);
	}
}

TEST(L1I, non_inclusive) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_NE(CPUINFO_CACHE_INCLUSIVE, l1i.instances[k].flags & CPUINFO_CACHE_INCLUSIVE);
	}
}

TEST(L1I, non_zero_processors) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_NE(0, l1i.instances[k].processor_count);
	}
}

TEST(L1I, valid_processors) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_LT(l1i.instances[k].processor_start, cpuinfo_processors_count);
		ASSERT_LE(l1i.instances[k].processor_start + l1i.instances[k].processor_count, cpuinfo_processors_count);
	}
}

TEST(L1D, non_zero_count) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_NE(0, l1d.count);
}

TEST(L1D, valid_count) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_LE(l1d.count, cpuinfo_processors_count);
}

TEST(L1D, non_null) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_TRUE(l1d.instances);
}

TEST(L1D, non_zero_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_NE(0, l1d.instances[k].size);
	}
}

TEST(L1D, valid_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(l1d.instances[k].size,
			l1d.instances[k].associativity * l1d.instances[k].sets * l1d.instances[k].partitions * l1d.instances[k].line_size);
	}
}

TEST(L1D, non_zero_associativity) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_NE(0, l1d.instances[k].associativity);
	}
}

TEST(L1D, non_zero_partitions) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_NE(0, l1d.instances[k].partitions);
	}
}

TEST(L1D, non_zero_line_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_NE(0, l1d.instances[k].line_size);
	}
}

TEST(L1D, power_of_2_line_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(0, l1d.instances[k].line_size & (l1d.instances[k].line_size - 1));
	}
}

TEST(L1D, valid_line_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_GE(l1d.instances[k].line_size, 16);
		ASSERT_LE(l1d.instances[k].line_size, 128);
	}
}

TEST(L1D, valid_flags) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(0, l1d.instances[k].flags & ~valid_flags);
	}
}

TEST(L1D, non_inclusive) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_NE(CPUINFO_CACHE_INCLUSIVE, l1d.instances[k].flags & CPUINFO_CACHE_INCLUSIVE);
	}
}

TEST(L1D, non_zero_processors) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_NE(0, l1d.instances[k].processor_count);
	}
}

TEST(L1D, valid_processors) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_LT(l1d.instances[k].processor_start, cpuinfo_processors_count);
		ASSERT_LE(l1d.instances[k].processor_start + l1d.instances[k].processor_count, cpuinfo_processors_count);
	}
}

TEST(L2, valid_count) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	ASSERT_LE(l2.count, cpuinfo_processors_count);
}

TEST(L2, non_null) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	if (l2.count != 0) {
		ASSERT_TRUE(l2.instances);
	}
}

TEST(L2, non_zero_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_NE(0, l2.instances[k].size);
	}
}

TEST(L2, valid_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(l2.instances[k].size,
			l2.instances[k].associativity * l2.instances[k].sets * l2.instances[k].partitions * l2.instances[k].line_size);
	}
}

TEST(L2, non_zero_associativity) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_NE(0, l2.instances[k].associativity);
	}
}

TEST(L2, non_zero_partitions) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_NE(0, l2.instances[k].partitions);
	}
}

TEST(L2, non_zero_line_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_NE(0, l2.instances[k].line_size);
	}
}

TEST(L2, power_of_2_line_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(0, l2.instances[k].line_size & (l2.instances[k].line_size - 1));
	}
}

TEST(L2, valid_line_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_GE(l2.instances[k].line_size, 16);
		ASSERT_LE(l2.instances[k].line_size, 128);
	}
}

TEST(L2, valid_flags) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(0, l2.instances[k].flags & ~valid_flags);
	}
}

TEST(L2, non_zero_processors) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_NE(0, l2.instances[k].processor_count);
	}
}

TEST(L2, valid_processors) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_LT(l2.instances[k].processor_start, cpuinfo_processors_count);
		ASSERT_LE(l2.instances[k].processor_start + l2.instances[k].processor_count, cpuinfo_processors_count);
	}
}

TEST(L3, valid_count) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	ASSERT_LE(l3.count, cpuinfo_processors_count);
}

TEST(L3, non_null) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	if (l3.count != 0) {
		ASSERT_TRUE(l3.instances);
	}
}

TEST(L3, non_zero_size) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_NE(0, l3.instances[k].size);
	}
}

TEST(L3, valid_size) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_EQ(l3.instances[k].size,
			l3.instances[k].associativity * l3.instances[k].sets * l3.instances[k].partitions * l3.instances[k].line_size);
	}
}

TEST(L3, non_zero_associativity) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_NE(0, l3.instances[k].associativity);
	}
}

TEST(L3, non_zero_partitions) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_NE(0, l3.instances[k].partitions);
	}
}

TEST(L3, non_zero_line_size) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_NE(0, l3.instances[k].line_size);
	}
}

TEST(L3, power_of_2_line_size) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_EQ(0, l3.instances[k].line_size & (l3.instances[k].line_size - 1));
	}
}

TEST(L3, valid_line_size) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_GE(l3.instances[k].line_size, 16);
		ASSERT_LE(l3.instances[k].line_size, 128);
	}
}

TEST(L3, valid_flags) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_EQ(0, l3.instances[k].flags & ~valid_flags);
	}
}

TEST(L3, non_zero_processors) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_NE(0, l3.instances[k].processor_count);
	}
}

TEST(L3, valid_processors) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	for (uint32_t k = 0; k < l3.count; k++) {
		ASSERT_LT(l3.instances[k].processor_start, cpuinfo_processors_count);
		ASSERT_LE(l3.instances[k].processor_start + l3.instances[k].processor_count, cpuinfo_processors_count);
	}
}

TEST(L4, valid_count) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	ASSERT_LE(l4.count, cpuinfo_processors_count);
}

TEST(L4, non_null) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	if (l4.count != 0) {
		ASSERT_TRUE(l4.instances);
	}
}

TEST(L4, non_zero_size) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_NE(0, l4.instances[k].size);
	}
}

TEST(L4, valid_size) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_EQ(l4.instances[k].size,
			l4.instances[k].associativity * l4.instances[k].sets * l4.instances[k].partitions * l4.instances[k].line_size);
	}
}

TEST(L4, non_zero_associativity) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_NE(0, l4.instances[k].associativity);
	}
}

TEST(L4, non_zero_partitions) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_NE(0, l4.instances[k].partitions);
	}
}

TEST(L4, non_zero_line_size) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_NE(0, l4.instances[k].line_size);
	}
}

TEST(L4, power_of_2_line_size) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_EQ(0, l4.instances[k].line_size & (l4.instances[k].line_size - 1));
	}
}

TEST(L4, valid_line_size) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_GE(l4.instances[k].line_size, 16);
		ASSERT_LE(l4.instances[k].line_size, 128);
	}
}

TEST(L4, valid_flags) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_EQ(0, l4.instances[k].flags & ~valid_flags);
	}
}

TEST(L4, non_zero_processors) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_NE(0, l4.instances[k].processor_count);
	}
}

TEST(L4, valid_processors) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	for (uint32_t k = 0; k < l4.count; k++) {
		ASSERT_LT(l4.instances[k].processor_start, cpuinfo_processors_count);
		ASSERT_LE(l4.instances[k].processor_start + l4.instances[k].processor_count, cpuinfo_processors_count);
	}
}

int main(int argc, char* argv[]) {
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
