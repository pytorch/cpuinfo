#include <gtest/gtest.h>

#include <cpuinfo.h>
#include <cpuinfo-mock.h>


TEST(PROCESSORS, count) {
	ASSERT_EQ(4, cpuinfo_get_processors_count());
}

TEST(PROCESSORS, non_null) {
	ASSERT_TRUE(cpuinfo_get_processors());
}

TEST(PROCESSORS, core) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_core(i), cpuinfo_get_processor(i)->core);
	}
}

TEST(PROCESSORS, cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_cluster(0), cpuinfo_get_processor(i)->cluster);
	}
}

TEST(PROCESSORS, package) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_package(0), cpuinfo_get_processor(i)->package);
	}
}

TEST(PROCESSORS, linux_id) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(i, cpuinfo_get_processor(i)->linux_id);
	}
}

TEST(PROCESSORS, l1i) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l1i_cache(i), cpuinfo_get_processor(i)->cache.l1i);
	}
}

TEST(PROCESSORS, l1d) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l1d_cache(i), cpuinfo_get_processor(i)->cache.l1d);
	}
}

TEST(PROCESSORS, l2) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		ASSERT_EQ(cpuinfo_get_l2_cache(0), cpuinfo_get_processor(i)->cache.l2);
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
	ASSERT_EQ(4, cpuinfo_get_cores_count());
}

TEST(CORES, non_null) {
	ASSERT_TRUE(cpuinfo_get_cores());
}

TEST(CORES, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(i, cpuinfo_get_core(i)->processor_start);
	}
}

TEST(CORES, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_core(i)->processor_count);
	}
}

TEST(CORES, core_id) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(i, cpuinfo_get_core(i)->core_id);
	}
}

TEST(CORES, cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_get_cluster(0), cpuinfo_get_core(i)->cluster);
	}
}

TEST(CORES, package) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_get_package(0), cpuinfo_get_core(i)->package);
	}
}

TEST(CORES, vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_vendor_sifive, cpuinfo_get_core(i)->vendor);
	}
}

TEST(CORES, uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		ASSERT_EQ(cpuinfo_uarch_unknown, cpuinfo_get_core(i)->uarch);
	}
}

TEST(CLUSTERS, count) {
	ASSERT_EQ(1, cpuinfo_get_clusters_count());
}

TEST(CLUSTERS, non_null) {
	ASSERT_TRUE(cpuinfo_get_clusters());
}

TEST(CLUSTERS, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_cluster(i)->processor_start);
	}
}

TEST(CLUSTERS, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(4, cpuinfo_get_cluster(i)->processor_count);
	}
}

TEST(CLUSTERS, core_start) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_cluster(i)->core_start);
	}
}

TEST(CLUSTERS, core_count) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(4, cpuinfo_get_cluster(i)->core_count);
	}
}

TEST(CLUSTERS, cluster_id) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(i, cpuinfo_get_cluster(i)->cluster_id);
	}
}

TEST(CLUSTERS, package) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(cpuinfo_get_package(0), cpuinfo_get_cluster(i)->package);
	}
}

TEST(CLUSTERS, vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(cpuinfo_vendor_sifive, cpuinfo_get_cluster(i)->vendor);
	}
}

TEST(CLUSTERS, uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		ASSERT_EQ(cpuinfo_uarch_unknown, cpuinfo_get_cluster(i)->uarch);
	}
}

TEST(PACKAGES, count) {
	ASSERT_EQ(1, cpuinfo_get_packages_count());
}

TEST(PACKAGES, name) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ("",
			std::string(cpuinfo_get_package(i)->name,
				strnlen(cpuinfo_get_package(i)->name, CPUINFO_PACKAGE_NAME_MAX)));
	}
}

TEST(PACKAGES, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_package(i)->processor_start);
	}
}

TEST(PACKAGES, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(4, cpuinfo_get_package(i)->processor_count);
	}
}

TEST(PACKAGES, core_start) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_package(i)->core_start);
	}
}

TEST(PACKAGES, core_count) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(4, cpuinfo_get_package(i)->core_count);
	}
}

TEST(PACKAGES, cluster_start) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(0, cpuinfo_get_package(i)->cluster_start);
	}
}

TEST(PACKAGES, cluster_count) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		ASSERT_EQ(1, cpuinfo_get_package(i)->cluster_count);
	}
}

TEST(ISA, i) {
	ASSERT_TRUE(cpuinfo_isa.i);
}

TEST(ISA, m) {
	ASSERT_TRUE(cpuinfo_isa.m);
}

TEST(ISA, a) {
	ASSERT_TRUE(cpuinfo_isa.a);
}

TEST(ISA, f) {
	ASSERT_TRUE(cpuinfo_isa.f);
}

TEST(ISA, d) {
	ASSERT_TRUE(cpuinfo_isa.d);
}

TEST(ISA, c) {
	ASSERT_TRUE(cpuinfo_isa.c);
}

TEST(ISA, v) {
	ASSERT_FALSE(cpuinfo_isa.v);
}

#include <starfive-visionfive-v2.h>

int riscv_hwprobe(struct riscv_hwprobe* pairs, size_t pair_count,
                  size_t /* cpu_count */, unsigned long * /* cpus */,
                  unsigned int /* flags */) {
	for (size_t pair = 0; pair < pair_count; pair++) {
		switch (pairs[pair].key) {
		case RISCV_HWPROBE_KEY_MVENDORID:
			pairs[pair].value = 0x489;
			break;
		default:
			pairs[pair].key = -1;
			break;
		}
	}
	return 0;
}

int main(int argc, char* argv[]) {
	cpuinfo_mock_filesystem(filesystem);
	cpuinfo_set_hwcap(UINT32_C(0x0000112D));
	cpuinfo_set_riscv_hwprobe(&riscv_hwprobe);
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
