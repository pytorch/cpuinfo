#include <gtest/gtest.h>

#include <cpuinfo.h>
#include <cpuinfo-mock.h>


TEST(PROCESSORS, count) {
	ASSERT_EQ(2, cpuinfo_processors_count);
}

TEST(PROCESSORS, non_null) {
	ASSERT_TRUE(cpuinfo_processors);
}

TEST(PROCESSORS, vendor_qualcomm) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(cpuinfo_vendor_qualcomm, cpuinfo_processors[i].vendor);
	}
}

TEST(PROCESSORS, uarch_krait) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(cpuinfo_uarch_krait, cpuinfo_processors[i].uarch);
	}
}

TEST(ISA, thumb) {
	ASSERT_TRUE(cpuinfo_isa.thumb);
}

TEST(ISA, thumb2) {
	ASSERT_TRUE(cpuinfo_isa.thumb2);
}

TEST(ISA, thumbee) {
	ASSERT_FALSE(cpuinfo_isa.thumbee);
}

TEST(ISA, jazelle) {
	ASSERT_FALSE(cpuinfo_isa.jazelle);
}

TEST(ISA, armv5e) {
	ASSERT_TRUE(cpuinfo_isa.armv5e);
}

TEST(ISA, armv6) {
	ASSERT_TRUE(cpuinfo_isa.armv6);
}

TEST(ISA, armv6k) {
	ASSERT_TRUE(cpuinfo_isa.armv6k);
}

TEST(ISA, armv7) {
	ASSERT_TRUE(cpuinfo_isa.armv7);
}

TEST(ISA, armv7mp) {
	ASSERT_TRUE(cpuinfo_isa.armv7mp);
}

TEST(ISA, idiv) {
	ASSERT_TRUE(cpuinfo_isa.idiv);
}

TEST(ISA, vfpv2) {
	ASSERT_FALSE(cpuinfo_isa.vfpv2);
}

TEST(ISA, vfpv3) {
	ASSERT_TRUE(cpuinfo_isa.vfpv3);
}

TEST(ISA, d32) {
	ASSERT_TRUE(cpuinfo_isa.d32);
}

TEST(ISA, fp16) {
	ASSERT_TRUE(cpuinfo_isa.fp16);
}

TEST(ISA, fma) {
	ASSERT_TRUE(cpuinfo_isa.fma);
}

TEST(ISA, wmmx) {
	ASSERT_FALSE(cpuinfo_isa.wmmx);
}

TEST(ISA, wmmx2) {
	ASSERT_FALSE(cpuinfo_isa.wmmx2);
}

TEST(ISA, neon) {
	ASSERT_TRUE(cpuinfo_isa.neon);
}

TEST(ISA, aes) {
	ASSERT_FALSE(cpuinfo_isa.aes);
}

TEST(ISA, sha1) {
	ASSERT_FALSE(cpuinfo_isa.sha1);
}

TEST(ISA, sha2) {
	ASSERT_FALSE(cpuinfo_isa.sha2);
}

TEST(ISA, pmull) {
	ASSERT_FALSE(cpuinfo_isa.pmull);
}

TEST(ISA, crc32) {
	ASSERT_FALSE(cpuinfo_isa.crc32);
}

TEST(L1I, count) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_EQ(4, l1i.count);
}

TEST(L1I, non_null) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_TRUE(l1i.instances);
}

TEST(L1I, size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(16 * 1024, l1i.instances[k].size);
	}
}

TEST(L1I, associativity) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(4, l1i.instances[k].associativity);
	}
}

TEST(L1I, sets) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(l1i.instances[k].size,
			l1i.instances[k].sets * l1i.instances[k].line_size * l1i.instances[k].partitions * l1i.instances[k].associativity);
	}
}

TEST(L1I, partitions) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(1, l1i.instances[k].partitions);
	}
}

TEST(L1I, line_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(64, l1i.instances[k].line_size);
	}
}

TEST(L1I, flags) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(0, l1i.instances[k].flags);
	}
}

TEST(L1I, processors) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(k, l1i.instances[k].thread_start);
		ASSERT_EQ(1, l1i.instances[k].thread_count);
	}
}

TEST(L1D, count) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_EQ(4, l1d.count);
}

TEST(L1D, non_null) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_TRUE(l1d.instances);
}

TEST(L1D, size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(16 * 1024, l1d.instances[k].size);
	}
}

TEST(L1D, associativity) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(4, l1d.instances[k].associativity);
	}
}

TEST(L1D, sets) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(l1d.instances[k].size,
			l1d.instances[k].sets * l1d.instances[k].line_size * l1d.instances[k].partitions * l1d.instances[k].associativity);
	}
}

TEST(L1D, partitions) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(1, l1d.instances[k].partitions);
	}
}

TEST(L1D, line_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(64, l1d.instances[k].line_size);
	}
}

TEST(L1D, flags) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(0, l1d.instances[k].flags);
	}
}

TEST(L1D, processors) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(k, l1d.instances[k].thread_start);
		ASSERT_EQ(1, l1d.instances[k].thread_count);
	}
}

TEST(L2, count) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	ASSERT_EQ(1, l2.count);
}

TEST(L2, non_null) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	ASSERT_TRUE(l2.instances);
}

TEST(L2, size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		switch (k) {
			ASSERT_EQ(1 * 1024 * 1024, l2.instances[k].size);
		}
	}
}

TEST(L2, associativity) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(8, l2.instances[k].associativity);
	}
}

TEST(L2, sets) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(l2.instances[k].size,
			l2.instances[k].sets * l2.instances[k].line_size * l2.instances[k].partitions * l2.instances[k].associativity);
	}
}

TEST(L2, partitions) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(1, l2.instances[k].partitions);
	}
}

TEST(L2, line_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(128, l2.instances[k].line_size);
	}
}

TEST(L2, flags) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(0, l2.instances[k].flags);
	}
}

TEST(L2, processors) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(0, l2.instances[k].thread_start);
		ASSERT_EQ(2, l2.instances[k].thread_count);
	}
}

TEST(L3, none) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	ASSERT_EQ(0, l3.count);
	ASSERT_FALSE(l3.instances);
}

TEST(L4, none) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	ASSERT_EQ(0, l4.count);
	ASSERT_FALSE(l4.instances);
}

#include <galaxy-s3-us.h>

int main(int argc, char* argv[]) {
	cpuinfo_mock_filesystem(filesystem);
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
