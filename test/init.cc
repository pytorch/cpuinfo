#include <gtest/gtest.h>

#include <cpuinfo.h>


TEST(PROCESSORS_COUNT, non_zero) {
	ASSERT_NE(0, cpuinfo_get_processors_count());
}

TEST(PROCESSORS, non_null) {
	ASSERT_TRUE(cpuinfo_get_processors());
}

TEST(CORES_COUNT, non_zero) {
	ASSERT_NE(0, cpuinfo_get_cores_count());
}

TEST(CORES, non_null) {
	ASSERT_TRUE(cpuinfo_get_cores());
}

TEST(CORES, known_vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_NE(cpuinfo_vendor_unknown, cpuinfo_get_core(i)->vendor);
	}
}

TEST(CORES, known_uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_NE(cpuinfo_uarch_unknown, cpuinfo_get_core(i)->uarch);
	}
}

TEST(L1I, non_zero_count) {
	ASSERT_NE(0, cpuinfo_get_l1i_caches_count());
}

TEST(L1I, valid_count) {
	ASSERT_LE(cpuinfo_get_l1i_caches_count(), cpuinfo_get_processors_count());
}

TEST(L1I, non_null) {
	ASSERT_TRUE(cpuinfo_get_l1i_caches());
}

TEST(L1I, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1i_cache(i)->size);
	}
}

TEST(L1I, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l1i_cache(i)->size,
			cpuinfo_get_l1i_cache(i)->associativity * cpuinfo_get_l1i_cache(i)->sets * cpuinfo_get_l1i_cache(i)->partitions * cpuinfo_get_l1i_cache(i)->line_size);
	}
}

TEST(L1I, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1i_cache(i)->associativity);
	}
}

TEST(L1I, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1i_cache(i)->partitions);
	}
}

TEST(L1I, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1i_cache(i)->line_size);
	}
}

TEST(L1I, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l1i_cache(i)->line_size & (cpuinfo_get_l1i_cache(i)->line_size - 1));
	}
}

TEST(L1I, valid_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_GE(cpuinfo_get_l1i_cache(i)->line_size, 16);
		ASSERT_LE(cpuinfo_get_l1i_cache(i)->line_size, 128);
	}
}

TEST(L1I, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l1i_cache(i)->flags & ~valid_flags);
	}
}

TEST(L1I, non_inclusive) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_NE(CPUINFO_CACHE_INCLUSIVE, cpuinfo_get_l1i_cache(i)->flags & CPUINFO_CACHE_INCLUSIVE);
	}
}

TEST(L1I, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1i_cache(i)->processor_count);
	}
}

TEST(L1I, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_LT(cpuinfo_get_l1i_cache(i)->processor_start, cpuinfo_get_processors_count());
		ASSERT_LE(cpuinfo_get_l1i_cache(i)->processor_start + cpuinfo_get_l1i_cache(i)->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L1D, non_zero_count) {
	ASSERT_NE(0, cpuinfo_get_l1d_caches_count());
}

TEST(L1D, valid_count) {
	ASSERT_LE(cpuinfo_get_l1d_caches_count(), cpuinfo_get_processors_count());
}

TEST(L1D, non_null) {
	ASSERT_TRUE(cpuinfo_get_l1d_caches());
}

TEST(L1D, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1d_cache(i)->size);
	}
}

TEST(L1D, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l1d_cache(i)->size,
			cpuinfo_get_l1d_cache(i)->associativity * cpuinfo_get_l1d_cache(i)->sets * cpuinfo_get_l1d_cache(i)->partitions * cpuinfo_get_l1d_cache(i)->line_size);
	}
}

TEST(L1D, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1d_cache(i)->associativity);
	}
}

TEST(L1D, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1d_cache(i)->partitions);
	}
}

TEST(L1D, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1d_cache(i)->line_size);
	}
}

TEST(L1D, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l1d_cache(i)->line_size & (cpuinfo_get_l1d_cache(i)->line_size - 1));
	}
}

TEST(L1D, valid_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_GE(cpuinfo_get_l1d_cache(i)->line_size, 16);
		ASSERT_LE(cpuinfo_get_l1d_cache(i)->line_size, 128);
	}
}

TEST(L1D, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l1d_cache(i)->flags & ~valid_flags);
	}
}

TEST(L1D, non_inclusive) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_NE(CPUINFO_CACHE_INCLUSIVE, cpuinfo_get_l1d_cache(i)->flags & CPUINFO_CACHE_INCLUSIVE);
	}
}

TEST(L1D, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l1d_cache(i)->processor_count);
	}
}

TEST(L1D, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_LT(cpuinfo_get_l1d_cache(i)->processor_start, cpuinfo_get_processors_count());
		ASSERT_LE(cpuinfo_get_l1d_cache(i)->processor_start + cpuinfo_get_l1d_cache(i)->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L2, valid_count) {
	ASSERT_LE(cpuinfo_get_l2_caches_count(), cpuinfo_get_processors_count());
}

TEST(L2, non_null) {
	if (cpuinfo_get_l2_caches_count() != 0) {
		ASSERT_TRUE(cpuinfo_get_l2_caches());
	}
}

TEST(L2, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l2_cache(i)->size);
	}
}

TEST(L2, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l2_cache(i)->size,
			cpuinfo_get_l2_cache(i)->associativity * cpuinfo_get_l2_cache(i)->sets * cpuinfo_get_l2_cache(i)->partitions * cpuinfo_get_l2_cache(i)->line_size);
	}
}

TEST(L2, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l2_cache(i)->associativity);
	}
}

TEST(L2, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l2_cache(i)->partitions);
	}
}

TEST(L2, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l2_cache(i)->line_size);
	}
}

TEST(L2, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l2_cache(i)->line_size & (cpuinfo_get_l2_cache(i)->line_size - 1));
	}
}

TEST(L2, valid_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_GE(cpuinfo_get_l2_cache(i)->line_size, 16);
		ASSERT_LE(cpuinfo_get_l2_cache(i)->line_size, 128);
	}
}

TEST(L2, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l2_cache(i)->flags & ~valid_flags);
	}
}

TEST(L2, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_NE(0, cpuinfo_get_l2_cache(i)->processor_count);
	}
}

TEST(L2, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		ASSERT_LT(cpuinfo_get_l2_cache(i)->processor_start, cpuinfo_get_processors_count());
		ASSERT_LE(cpuinfo_get_l2_cache(i)->processor_start + cpuinfo_get_l2_cache(i)->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L3, valid_count) {
	ASSERT_LE(cpuinfo_get_l3_caches_count(), cpuinfo_get_processors_count());
}

TEST(L3, non_null) {
	if (cpuinfo_get_l3_caches_count() != 0) {
		ASSERT_TRUE(cpuinfo_get_l3_caches());
	}
}

TEST(L3, non_zero_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l3_caches()[k].size);
	}
}

TEST(L3, valid_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_EQ(cpuinfo_get_l3_caches()[k].size,
			cpuinfo_get_l3_caches()[k].associativity * cpuinfo_get_l3_caches()[k].sets * cpuinfo_get_l3_caches()[k].partitions * cpuinfo_get_l3_caches()[k].line_size);
	}
}

TEST(L3, non_zero_associativity) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l3_caches()[k].associativity);
	}
}

TEST(L3, non_zero_partitions) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l3_caches()[k].partitions);
	}
}

TEST(L3, non_zero_line_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l3_caches()[k].line_size);
	}
}

TEST(L3, power_of_2_line_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_EQ(0, cpuinfo_get_l3_caches()[k].line_size & (cpuinfo_get_l3_caches()[k].line_size - 1));
	}
}

TEST(L3, valid_line_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_GE(cpuinfo_get_l3_caches()[k].line_size, 16);
		ASSERT_LE(cpuinfo_get_l3_caches()[k].line_size, 128);
	}
}

TEST(L3, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_EQ(0, cpuinfo_get_l3_caches()[k].flags & ~valid_flags);
	}
}

TEST(L3, non_zero_processors) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l3_caches()[k].processor_count);
	}
}

TEST(L3, valid_processors) {
	for (uint32_t k = 0; k < cpuinfo_get_l3_caches_count(); k++) {
		ASSERT_LT(cpuinfo_get_l3_caches()[k].processor_start, cpuinfo_get_processors_count());
		ASSERT_LE(cpuinfo_get_l3_caches()[k].processor_start + cpuinfo_get_l3_caches()[k].processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L4, valid_count) {
	ASSERT_LE(cpuinfo_get_l4_caches_count(), cpuinfo_get_processors_count());
}

TEST(L4, non_null) {
	if (cpuinfo_get_l4_caches_count() != 0) {
		ASSERT_TRUE(cpuinfo_get_l4_caches());
	}
}

TEST(L4, non_zero_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l4_caches()[k].size);
	}
}

TEST(L4, valid_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_EQ(cpuinfo_get_l4_caches()[k].size,
			cpuinfo_get_l4_caches()[k].associativity * cpuinfo_get_l4_caches()[k].sets * cpuinfo_get_l4_caches()[k].partitions * cpuinfo_get_l4_caches()[k].line_size);
	}
}

TEST(L4, non_zero_associativity) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l4_caches()[k].associativity);
	}
}

TEST(L4, non_zero_partitions) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l4_caches()[k].partitions);
	}
}

TEST(L4, non_zero_line_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l4_caches()[k].line_size);
	}
}

TEST(L4, power_of_2_line_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_EQ(0, cpuinfo_get_l4_caches()[k].line_size & (cpuinfo_get_l4_caches()[k].line_size - 1));
	}
}

TEST(L4, valid_line_size) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_GE(cpuinfo_get_l4_caches()[k].line_size, 16);
		ASSERT_LE(cpuinfo_get_l4_caches()[k].line_size, 128);
	}
}

TEST(L4, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_EQ(0, cpuinfo_get_l4_caches()[k].flags & ~valid_flags);
	}
}

TEST(L4, non_zero_processors) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_NE(0, cpuinfo_get_l4_caches()[k].processor_count);
	}
}

TEST(L4, valid_processors) {
	for (uint32_t k = 0; k < cpuinfo_get_l4_caches_count(); k++) {
		ASSERT_LT(cpuinfo_get_l4_caches()[k].processor_start, cpuinfo_get_processors_count());
		ASSERT_LE(cpuinfo_get_l4_caches()[k].processor_start + cpuinfo_get_l4_caches()[k].processor_count, cpuinfo_get_processors_count());
	}
}

int main(int argc, char* argv[]) {
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}