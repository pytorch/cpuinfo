#include <gtest/gtest.h>

#include <cpuinfo-mock.h>
#include <cpuinfo.h>

TEST(PROCESSORS, count) {
	ASSERT_EQ(8, cpuinfo_get_processors_count());
}

TEST(PROCESSORS, non_null) {
	ASSERT_TRUE(cpuinfo_get_processors());
}

TEST(PROCESSORS, smt_id) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(i % 1, cpuinfo_get_processor(i)->smt_id);
	}
}

TEST(PROCESSORS, core) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_core(i / 1), cpuinfo_get_processor(i)->core);
	}
}

TEST(PROCESSORS, cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_cluster(i / 1), cpuinfo_get_processor(i)->cluster);
	}
}

TEST(PROCESSORS, package) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_package(i / 1), cpuinfo_get_processor(i)->package);
	}
}

TEST(PROCESSORS, linux_id) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(i, cpuinfo_get_processor(i)->linux_id);
	}
}

TEST(PROCESSORS, l1i) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l1i_cache(i / 1), cpuinfo_get_processor(i)->cache.l1i);
	}
}

TEST(PROCESSORS, l1d) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l1d_cache(i / 1), cpuinfo_get_processor(i)->cache.l1d);
	}
}

TEST(PROCESSORS, l2) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_FALSE(cpuinfo_get_processor(i)->cache.l2);
	}
}

TEST(PROCESSORS, l3) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_FALSE(cpuinfo_get_processor(i)->cache.l3);
	}
}

TEST(PROCESSORS, l4) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_FALSE(cpuinfo_get_processor(i)->cache.l4);
	}
}

TEST(CORES, count) {
	ASSERT_EQ(8, cpuinfo_get_cores_count());
}

TEST(CORES, non_null) {
	ASSERT_TRUE(cpuinfo_get_cores());
}

TEST(CORES, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_core(i)->processor_start);
	}
}

TEST(CORES, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_core(i)->processor_count);
	}
}

TEST(CORES, core_id) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(i % 1, cpuinfo_get_core(i)->core_id);
	}
}

TEST(CORES, cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_get_cluster(i / 1), cpuinfo_get_core(i)->cluster);
	}
}

TEST(CORES, package) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_get_package(i / 1), cpuinfo_get_core(i)->package);
	}
}

TEST(CORES, vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_vendor_ibm, cpuinfo_get_core(i)->vendor);
	}
}

TEST(CORES, uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_uarch_power10, cpuinfo_get_core(i)->uarch);
	}
}

TEST(CORES, pvr) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(UINT32_C(0x00800200), cpuinfo_get_core(i)->pvr);
	}
}

TEST(CLUSTERS, count) {
	ASSERT_EQ(8, cpuinfo_get_clusters_count());
}

TEST(CLUSTERS, non_null) {
	ASSERT_TRUE(cpuinfo_get_clusters());
}

TEST(CLUSTERS, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_cluster(i)->processor_start);
	}
}

TEST(CLUSTERS, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_cluster(i)->processor_count);
	}
}

TEST(CLUSTERS, core_start) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_cluster(i)->core_start);
	}
}

TEST(CLUSTERS, core_count) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_cluster(i)->core_count);
	}
}

TEST(CLUSTERS, cluster_id) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(i % 1, cpuinfo_get_cluster(i)->cluster_id);
	}
}

TEST(CLUSTERS, package) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(cpuinfo_get_package(i / 1), cpuinfo_get_cluster(i)->package);
	}
}

TEST(CLUSTERS, vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(cpuinfo_vendor_ibm, cpuinfo_get_cluster(i)->vendor);
	}
}

TEST(CLUSTERS, uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(cpuinfo_uarch_power10, cpuinfo_get_cluster(i)->uarch);
	}
}

TEST(CLUSTERS, pvr) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(UINT32_C(0x00800200), cpuinfo_get_cluster(i)->pvr);
	}
}

TEST(PACKAGES, count) {
	ASSERT_EQ(8, cpuinfo_get_packages_count());
}

TEST(PACKAGES, non_null) {
	ASSERT_TRUE(cpuinfo_get_packages());
}

TEST(PACKAGES, name) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(
			"POWER10",
			std::string(
				cpuinfo_get_package(i)->name,
				strnlen(cpuinfo_get_package(i)->name, CPUINFO_PACKAGE_NAME_MAX)));
	}
}

TEST(PACKAGES, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_package(i)->processor_start);
	}
}

TEST(PACKAGES, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_package(i)->processor_count);
	}
}

TEST(PACKAGES, core_start) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_package(i)->core_start);
	}
}

TEST(PACKAGES, core_count) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_package(i)->core_count);
	}
}

TEST(PACKAGES, cluster_start) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_package(i)->cluster_start);
	}
}

TEST(PACKAGES, cluster_count) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_package(i)->cluster_count);
	}
}

TEST(ISA, altivec) {
	ASSERT_TRUE(cpuinfo_has_powerpc_altivec());
}

TEST(ISA, vsx) {
	ASSERT_TRUE(cpuinfo_has_powerpc_vsx());
}

TEST(ISA, dfp) {
	ASSERT_TRUE(cpuinfo_has_powerpc_dfp());
}

TEST(ISA, isa_v2_06) {
	ASSERT_TRUE(cpuinfo_has_powerpc_isa_v2_06());
}

TEST(ISA, isa_v2_07) {
	ASSERT_TRUE(cpuinfo_has_powerpc_isa_v2_07());
}

TEST(ISA, isa_v3_00) {
	ASSERT_TRUE(cpuinfo_has_powerpc_isa_v3_00());
}

TEST(ISA, isa_v3_1) {
	ASSERT_TRUE(cpuinfo_has_powerpc_isa_v3_1());
}

TEST(ISA, htm) {
	ASSERT_FALSE(cpuinfo_has_powerpc_htm());
}

TEST(ISA, vec_crypto) {
	ASSERT_TRUE(cpuinfo_has_powerpc_vec_crypto());
}

TEST(ISA, ieee128) {
	ASSERT_TRUE(cpuinfo_has_powerpc_ieee128());
}

TEST(ISA, darn) {
	ASSERT_TRUE(cpuinfo_has_powerpc_darn());
}

TEST(ISA, mma) {
	ASSERT_TRUE(cpuinfo_has_powerpc_mma());
}

TEST(L1I, count) {
	ASSERT_EQ(8, cpuinfo_get_l1i_caches_count());
}

TEST(L1I, non_null) {
	ASSERT_TRUE(cpuinfo_get_l1i_caches());
}

TEST(L1I, size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(48 * 1024, cpuinfo_get_l1i_cache(i)->size);
	}
}

TEST(L1I, associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(6, cpuinfo_get_l1i_cache(i)->associativity);
	}
}

TEST(L1I, sets) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(
			cpuinfo_get_l1i_cache(i)->size,
			cpuinfo_get_l1i_cache(i)->sets * cpuinfo_get_l1i_cache(i)->line_size *
				cpuinfo_get_l1i_cache(i)->partitions * cpuinfo_get_l1i_cache(i)->associativity);
	}
}

TEST(L1I, partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_l1i_cache(i)->partitions);
	}
}

TEST(L1I, line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(128, cpuinfo_get_l1i_cache(i)->line_size);
	}
}

TEST(L1I, flags) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l1i_cache(i)->flags);
	}
}

TEST(L1I, processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_l1i_cache(i)->processor_start);
		ASSERT_EQ(1, cpuinfo_get_l1i_cache(i)->processor_count);
	}
}

TEST(L1D, count) {
	ASSERT_EQ(8, cpuinfo_get_l1d_caches_count());
}

TEST(L1D, non_null) {
	ASSERT_TRUE(cpuinfo_get_l1d_caches());
}

TEST(L1D, size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(32 * 1024, cpuinfo_get_l1d_cache(i)->size);
	}
}

TEST(L1D, associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(8, cpuinfo_get_l1d_cache(i)->associativity);
	}
}

TEST(L1D, sets) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(
			cpuinfo_get_l1d_cache(i)->size,
			cpuinfo_get_l1d_cache(i)->sets * cpuinfo_get_l1d_cache(i)->line_size *
				cpuinfo_get_l1d_cache(i)->partitions * cpuinfo_get_l1d_cache(i)->associativity);
	}
}

TEST(L1D, partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_l1d_cache(i)->partitions);
	}
}

TEST(L1D, line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(128, cpuinfo_get_l1d_cache(i)->line_size);
	}
}

TEST(L1D, flags) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_l1d_cache(i)->flags);
	}
}

TEST(L1D, processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		ASSERT_EQ(i * 1, cpuinfo_get_l1d_cache(i)->processor_start);
		ASSERT_EQ(1, cpuinfo_get_l1d_cache(i)->processor_count);
	}
}

TEST(L2, none) {
	ASSERT_EQ(0, cpuinfo_get_l2_caches_count());
	ASSERT_FALSE(cpuinfo_get_l2_caches());
}

TEST(L3, none) {
	ASSERT_EQ(0, cpuinfo_get_l3_caches_count());
	ASSERT_FALSE(cpuinfo_get_l3_caches());
}

TEST(L4, none) {
	ASSERT_EQ(0, cpuinfo_get_l4_caches_count());
	ASSERT_FALSE(cpuinfo_get_l4_caches());
}

#include <power10-kvm.h>

int main(int argc, char* argv[]) {
	cpuinfo_set_hwcap(UINT64_C(0xDC0065C2));
	cpuinfo_set_hwcap2(UINT64_C(0xBEF60000));
	cpuinfo_mock_filesystem(filesystem);
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
