#include <gtest/gtest.h>

#include <cpuinfo.h>


TEST(PROCESSORS_COUNT, non_zero) {
	EXPECT_NE(0, cpuinfo_get_processors_count());
}

TEST(PROCESSORS, non_null) {
	EXPECT_TRUE(cpuinfo_get_processors());
}

TEST(PROCESSOR, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_processor(i));
	}
}

TEST(PROCESSOR, valid_smt_id) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_core* core = processor->core;
		ASSERT_TRUE(core);

		EXPECT_LT(processor->smt_id, core->processor_count);
	}
}

TEST(PROCESSOR, valid_core) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);

		EXPECT_TRUE(processor->core);
	}
}

TEST(PROCESSOR, consistent_core) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_core* core = processor->core;
		ASSERT_TRUE(core);

		EXPECT_GE(i, core->processor_start);
		EXPECT_LT(i, core->processor_start + core->processor_count);
	}
}

TEST(PROCESSOR, valid_cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);

		EXPECT_TRUE(processor->cluster);
	}
}

TEST(PROCESSOR, consistent_cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_cluster* cluster = processor->cluster;
		ASSERT_TRUE(cluster);

		EXPECT_GE(i, cluster->processor_start);
		EXPECT_LT(i, cluster->processor_start + cluster->processor_count);
	}
}

TEST(PROCESSOR, valid_package) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);

		EXPECT_TRUE(processor->package);
	}
}

TEST(PROCESSOR, consistent_package) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_package* package = processor->package;
		ASSERT_TRUE(package);

		EXPECT_GE(i, package->processor_start);
		EXPECT_LT(i, package->processor_start + package->processor_count);
	}
}

TEST(PROCESSOR, consistent_l1i) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_cache* l1i = processor->cache.l1i;
		if (l1i != nullptr) {
			EXPECT_GE(i, l1i->processor_start);
			EXPECT_LT(i, l1i->processor_start + l1i->processor_count);
		}
	}
}

TEST(PROCESSOR, consistent_l1d) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_cache* l1d = processor->cache.l1d;
		if (l1d != nullptr) {
			EXPECT_GE(i, l1d->processor_start);
			EXPECT_LT(i, l1d->processor_start + l1d->processor_count);
		}
	}
}

TEST(PROCESSOR, consistent_l2) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_cache* l2 = processor->cache.l2;
		if (l2 != nullptr) {
			EXPECT_GE(i, l2->processor_start);
			EXPECT_LT(i, l2->processor_start + l2->processor_count);
		}
	}
}

TEST(PROCESSOR, consistent_l3) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_cache* l3 = processor->cache.l3;
		if (l3 != nullptr) {
			EXPECT_GE(i, l3->processor_start);
			EXPECT_LT(i, l3->processor_start + l3->processor_count);
		}
	}
}

TEST(PROCESSOR, consistent_l4) {
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const cpuinfo_processor* processor = cpuinfo_get_processor(i);
		ASSERT_TRUE(processor);
		const cpuinfo_cache* l4 = processor->cache.l4;
		if (l4 != nullptr) {
			EXPECT_GE(i, l4->processor_start);
			EXPECT_LT(i, l4->processor_start + l4->processor_count);
		}
	}
}

TEST(CORES_COUNT, within_bounds) {
	EXPECT_NE(0, cpuinfo_get_cores_count());
	EXPECT_LE(cpuinfo_get_cores_count(), cpuinfo_get_processors_count());
	EXPECT_GE(cpuinfo_get_cores_count(), cpuinfo_get_packages_count());
}

TEST(CORES, non_null) {
	EXPECT_TRUE(cpuinfo_get_cores());
}

TEST(CORE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_core(i));
	}
}

TEST(CORE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);

		EXPECT_NE(0, core->processor_count);
	}
}

TEST(CORE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);

		for (uint32_t i = 0; i < core->processor_count; i++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(core->processor_start + i);
			ASSERT_TRUE(processor);

			EXPECT_EQ(core, processor->core);
		}
	}
}

TEST(CORE, valid_core_id) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);
		const cpuinfo_package* package = core->package;
		ASSERT_TRUE(package);

		EXPECT_LT(core->core_id, package->core_count);
	}
}

TEST(CORE, valid_cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);

		EXPECT_TRUE(core->cluster);
	}
}

TEST(CORE, consistent_cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);
		const cpuinfo_cluster* cluster = core->cluster;
		ASSERT_TRUE(cluster);

		EXPECT_GE(i, cluster->core_start);
		EXPECT_LT(i, cluster->core_start + cluster->core_count);
	}
}

TEST(CORE, valid_package) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);

		EXPECT_TRUE(core->package);
	}
}

TEST(CORE, consistent_package) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);
		const cpuinfo_package* package = core->package;
		ASSERT_TRUE(package);

		EXPECT_GE(i, package->core_start);
		EXPECT_LT(i, package->core_start + package->core_count);
	}
}

TEST(CORE, known_vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);

		EXPECT_NE(cpuinfo_vendor_unknown, core->vendor);
	}
}

TEST(CORE, known_uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const cpuinfo_core* core = cpuinfo_get_core(i);
		ASSERT_TRUE(core);

		EXPECT_NE(cpuinfo_uarch_unknown, core->uarch);
	}
}

TEST(CLUSTERS_COUNT, within_bounds) {
	EXPECT_NE(0, cpuinfo_get_clusters_count());
	EXPECT_LE(cpuinfo_get_clusters_count(), cpuinfo_get_cores_count());
	EXPECT_LE(cpuinfo_get_clusters_count(), cpuinfo_get_processors_count());
	EXPECT_GE(cpuinfo_get_clusters_count(), cpuinfo_get_packages_count());
}

TEST(CLUSTERS, non_null) {
	EXPECT_TRUE(cpuinfo_get_clusters());
}

TEST(CLUSTER, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_cluster(i));
	}
}

TEST(CLUSTER, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		EXPECT_NE(0, cluster->processor_count);
	}
}

TEST(CLUSTER, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		EXPECT_LT(cluster->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(cluster->processor_start + cluster->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(CLUSTER, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(cluster->processor_start + j);
			EXPECT_EQ(cluster, processor->cluster);
		}
	}
}

TEST(CLUSTER, non_zero_cores) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		EXPECT_NE(0, cluster->core_count);
	}
}

TEST(CLUSTER, valid_cores) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		EXPECT_LT(cluster->core_start, cpuinfo_get_cores_count());
		EXPECT_LE(cluster->core_start + cluster->core_count, cpuinfo_get_cores_count());
	}
}

TEST(CLUSTER, consistent_cores) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(cluster->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(cluster, core->cluster);
		}
	}
}

TEST(CLUSTER, valid_cluster_id) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_package* package = cluster->package;
			ASSERT_TRUE(package);

			EXPECT_LT(cluster->cluster_id, package->cluster_count);
		}
	}
}

TEST(CLUSTER, valid_package) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		EXPECT_TRUE(cluster->package);
	}
}

TEST(CLUSTER, consistent_package) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);
		const cpuinfo_package* package = cluster->package;
		ASSERT_TRUE(package);

		EXPECT_GE(i, package->cluster_start);
		EXPECT_LT(i, package->cluster_start + package->cluster_count);
	}
}

TEST(CLUSTER, consistent_vendor) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(cluster->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(cluster->vendor, core->vendor);
		}
	}
}

TEST(CLUSTER, consistent_uarch) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(cluster->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(cluster->uarch, core->uarch);
		}
	}
}

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
TEST(CLUSTER, consistent_cpuid) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(cluster->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(cluster->cpuid, core->cpuid);
		}
	}
}
#endif /* CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64 */

#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
TEST(CLUSTER, consistent_midr) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(cluster->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(cluster->midr, core->midr);
		}
	}
}
#endif /* CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64 */

TEST(CLUSTER, consistent_frequency) {
	for (uint32_t i = 0; i < cpuinfo_get_clusters_count(); i++) {
		const cpuinfo_cluster* cluster = cpuinfo_get_cluster(i);
		ASSERT_TRUE(cluster);

		for (uint32_t j = 0; j < cluster->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(cluster->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(cluster->frequency, core->frequency);
		}
	}
}

TEST(PACKAGES_COUNT, within_bounds) {
	EXPECT_NE(0, cpuinfo_get_packages_count());
	EXPECT_LE(cpuinfo_get_packages_count(), cpuinfo_get_cores_count());
	EXPECT_LE(cpuinfo_get_packages_count(), cpuinfo_get_processors_count());
}

TEST(PACKAGES, non_null) {
	EXPECT_TRUE(cpuinfo_get_packages());
}

TEST(PACKAGE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_package(i));
	}
}

TEST(PACKAGE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		EXPECT_NE(0, package->processor_count);
	}
}

TEST(PACKAGE, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		EXPECT_LT(package->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(package->processor_start + package->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(PACKAGE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		for (uint32_t j = 0; j < package->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(package->processor_start + j);
			ASSERT_TRUE(processor);

			EXPECT_EQ(package, processor->package);
		}
	}
}

TEST(PACKAGE, non_zero_cores) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		EXPECT_NE(0, package->core_count);
	}
}

TEST(PACKAGE, valid_cores) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		EXPECT_LT(package->core_start, cpuinfo_get_cores_count());
		EXPECT_LE(package->core_start + package->core_count, cpuinfo_get_cores_count());
	}
}

TEST(PACKAGE, consistent_cores) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		for (uint32_t j = 0; j < package->core_count; j++) {
			const cpuinfo_core* core = cpuinfo_get_core(package->core_start + j);
			ASSERT_TRUE(core);

			EXPECT_EQ(package, core->package);
		}
	}
}

TEST(PACKAGE, non_zero_clusters) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		EXPECT_NE(0, package->cluster_count);
	}
}

TEST(PACKAGE, valid_clusters) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		EXPECT_LT(package->cluster_start, cpuinfo_get_clusters_count());
		EXPECT_LE(package->cluster_start + package->cluster_count, cpuinfo_get_clusters_count());
	}
}

TEST(PACKAGE, consistent_cluster) {
	for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
		const cpuinfo_package* package = cpuinfo_get_package(i);
		ASSERT_TRUE(package);

		for (uint32_t j = 0; j < package->cluster_count; j++) {
			const cpuinfo_cluster* cluster = cpuinfo_get_cluster(package->cluster_start + j);
			ASSERT_TRUE(cluster);

			EXPECT_EQ(package, cluster->package);
		}
	}
}

TEST(L1I_CACHES_COUNT, within_bounds) {
	EXPECT_NE(0, cpuinfo_get_l1i_caches_count());
	EXPECT_LE(cpuinfo_get_l1i_caches_count(), cpuinfo_get_processors_count());
}

TEST(L1I_CACHES, non_null) {
	EXPECT_TRUE(cpuinfo_get_l1i_caches());
}

TEST(L1I_CACHE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_l1i_cache(i));
	}
}

TEST(L1I_CACHE, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->size);
	}
}

TEST(L1I_CACHE, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(cache->size,
			cache->associativity * cache->sets * cache->partitions * cache->line_size);
	}
}

TEST(L1I_CACHE, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->associativity);
	}
}

TEST(L1I_CACHE, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->partitions);
	}
}

TEST(L1I_CACHE, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->line_size);
	}
}

TEST(L1I_CACHE, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		const uint32_t line_size = cache->line_size;
		EXPECT_NE(0, line_size);
		EXPECT_EQ(0, line_size & (line_size - 1));
	}
}

TEST(L1I_CACHE, reasonable_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_GE(cache->line_size, 16);
		EXPECT_LE(cache->line_size, 128);
	}
}

TEST(L1I_CACHE, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(0, cache->flags & ~valid_flags);
	}
}

TEST(L1I_CACHE, non_inclusive) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(CPUINFO_CACHE_INCLUSIVE, cache->flags & CPUINFO_CACHE_INCLUSIVE);
	}
}

TEST(L1I_CACHE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->processor_count);
	}
}

TEST(L1I_CACHE, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_LT(cache->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(cache->processor_start + cache->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L1I_CACHE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1i_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1i_cache(i);
		ASSERT_TRUE(cache);

		for (uint32_t j = 0; j < cache->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(cache->processor_start + j);
			ASSERT_TRUE(processor);

			EXPECT_EQ(cache, processor->cache.l1i);
		}
	}
}

TEST(L1D_CACHES_COUNT, within_bounds) {
	EXPECT_NE(0, cpuinfo_get_l1d_caches_count());
	EXPECT_LE(cpuinfo_get_l1d_caches_count(), cpuinfo_get_processors_count());
}

TEST(L1D_CACHES, non_null) {
	EXPECT_TRUE(cpuinfo_get_l1d_caches());
}

TEST(L1D_CACHE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_l1d_cache(i));
	}
}

TEST(L1D_CACHE, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->size);
	}
}

TEST(L1D_CACHE, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(cache->size,
			cache->associativity * cache->sets * cache->partitions * cache->line_size);
	}
}

TEST(L1D_CACHE, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->associativity);
	}
}

TEST(L1D_CACHE, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->partitions);
	}
}

TEST(L1D_CACHE, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->line_size);
	}
}

TEST(L1D_CACHE, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		const uint32_t line_size = cache->line_size;
		EXPECT_NE(0, line_size);
		EXPECT_EQ(0, line_size & (line_size - 1));
	}
}

TEST(L1D_CACHE, reasonable_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_GE(cache->line_size, 16);
		EXPECT_LE(cache->line_size, 128);
	}
}

TEST(L1D_CACHE, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(0, cache->flags & ~valid_flags);
	}
}

TEST(L1D_CACHE, non_inclusive) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(CPUINFO_CACHE_INCLUSIVE, cache->flags & CPUINFO_CACHE_INCLUSIVE);
	}
}

TEST(L1D_CACHE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->processor_count);
	}
}

TEST(L1D_CACHE, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_LT(cache->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(cache->processor_start + cache->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L1D_CACHE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l1d_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l1d_cache(i);
		ASSERT_TRUE(cache);

		for (uint32_t j = 0; j < cache->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(cache->processor_start + j);
			ASSERT_TRUE(processor);

			EXPECT_EQ(cache, processor->cache.l1d);
		}
	}
}

TEST(L2_CACHES_COUNT, within_bounds) {
	EXPECT_NE(0, cpuinfo_get_l2_caches_count());
	EXPECT_LE(cpuinfo_get_l2_caches_count(), cpuinfo_get_processors_count());
	EXPECT_LE(cpuinfo_get_l2_caches_count(), cpuinfo_get_l1d_caches_count());
	EXPECT_LE(cpuinfo_get_l2_caches_count(), cpuinfo_get_l1i_caches_count());
}

TEST(L2_CACHES, non_null) {
	EXPECT_TRUE(cpuinfo_get_l2_caches());
}

TEST(L2_CACHE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_l2_cache(i));
	}
}

TEST(L2_CACHE, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->size);
	}
}

TEST(L2_CACHE, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(cache->size,
			cache->associativity * cache->sets * cache->partitions * cache->line_size);
	}
}

TEST(L2_CACHE, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->associativity);
	}
}

TEST(L2_CACHE, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->partitions);
	}
}

TEST(L2_CACHE, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->line_size);
	}
}

TEST(L2_CACHE, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		const uint32_t line_size = cache->line_size;
		EXPECT_NE(0, line_size);
		EXPECT_EQ(0, line_size & (line_size - 1));
	}
}

TEST(L2_CACHE, reasonable_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_GE(cache->line_size, 16);
		EXPECT_LE(cache->line_size, 128);
	}
}

TEST(L2_CACHE, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(0, cache->flags & ~valid_flags);
	}
}

TEST(L2_CACHE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->processor_count);
	}
}

TEST(L2_CACHE, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_LT(cache->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(cache->processor_start + cache->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L2_CACHE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l2_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l2_cache(i);
		ASSERT_TRUE(cache);

		for (uint32_t j = 0; j < cache->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(cache->processor_start + j);
			ASSERT_TRUE(processor);

			EXPECT_EQ(cache, processor->cache.l2);
		}
	}
}

TEST(L3_CACHES_COUNT, within_bounds) {
	EXPECT_LE(cpuinfo_get_l3_caches_count(), cpuinfo_get_processors_count());
	EXPECT_LE(cpuinfo_get_l3_caches_count(), cpuinfo_get_l2_caches_count());
}

TEST(L3_CACHE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_l3_cache(i));
	}
}

TEST(L3_CACHE, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->size);
	}
}

TEST(L3_CACHE, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(cache->size,
			cache->associativity * cache->sets * cache->partitions * cache->line_size);
	}
}

TEST(L3_CACHE, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->associativity);
	}
}

TEST(L3_CACHE, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->partitions);
	}
}

TEST(L3_CACHE, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->line_size);
	}
}

TEST(L3_CACHE, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		const uint32_t line_size = cache->line_size;
		EXPECT_NE(0, line_size);
		EXPECT_EQ(0, line_size & (line_size - 1));
	}
}

TEST(L3_CACHE, reasonable_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_GE(cache->line_size, 16);
		EXPECT_LE(cache->line_size, 128);
	}
}

TEST(L3_CACHE, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(0, cache->flags & ~valid_flags);
	}
}

TEST(L3_CACHE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->processor_count);
	}
}

TEST(L3_CACHE, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_LT(cache->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(cache->processor_start + cache->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L3_CACHE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l3_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l3_cache(i);
		ASSERT_TRUE(cache);

		for (uint32_t j = 0; j < cache->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(cache->processor_start + j);
			ASSERT_TRUE(processor);

			EXPECT_EQ(cache, processor->cache.l3);
		}
	}
}

TEST(L4_CACHES_COUNT, within_bounds) {
	EXPECT_LE(cpuinfo_get_l4_caches_count(), cpuinfo_get_processors_count());
	EXPECT_LE(cpuinfo_get_l4_caches_count(), cpuinfo_get_l3_caches_count());
}

TEST(L4_CACHE, non_null) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		EXPECT_TRUE(cpuinfo_get_l4_cache(i));
	}
}

TEST(L4_CACHE, non_zero_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->size);
	}
}

TEST(L4_CACHE, valid_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(cache->size,
			cache->associativity * cache->sets * cache->partitions * cache->line_size);
	}
}

TEST(L4_CACHE, non_zero_associativity) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->associativity);
	}
}

TEST(L4_CACHE, non_zero_partitions) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->partitions);
	}
}

TEST(L4_CACHE, non_zero_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->line_size);
	}
}

TEST(L4_CACHE, power_of_2_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		const uint32_t line_size = cache->line_size;
		EXPECT_NE(0, line_size);
		EXPECT_EQ(0, line_size & (line_size - 1));
	}
}

TEST(L4_CACHE, reasonable_line_size) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_GE(cache->line_size, 16);
		EXPECT_LE(cache->line_size, 128);
	}
}

TEST(L4_CACHE, valid_flags) {
	const uint32_t valid_flags = CPUINFO_CACHE_UNIFIED | CPUINFO_CACHE_INCLUSIVE | CPUINFO_CACHE_COMPLEX_INDEXING;
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_EQ(0, cache->flags & ~valid_flags);
	}
}

TEST(L4_CACHE, non_zero_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_NE(0, cache->processor_count);
	}
}

TEST(L4_CACHE, valid_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		EXPECT_LT(cache->processor_start, cpuinfo_get_processors_count());
		EXPECT_LE(cache->processor_start + cache->processor_count, cpuinfo_get_processors_count());
	}
}

TEST(L4_CACHE, consistent_processors) {
	for (uint32_t i = 0; i < cpuinfo_get_l4_caches_count(); i++) {
		const cpuinfo_cache* cache = cpuinfo_get_l4_cache(i);
		ASSERT_TRUE(cache);

		for (uint32_t j = 0; j < cache->processor_count; j++) {
			const cpuinfo_processor* processor = cpuinfo_get_processor(cache->processor_start + j);
			ASSERT_TRUE(processor);

			EXPECT_EQ(cache, processor->cache.l4);
		}
	}
}

int main(int argc, char* argv[]) {
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}