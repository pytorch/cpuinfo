#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo.h>
#include <loongarch/linux/api.h>
#include <loongarch/api.h>
#include <loongarch/cpucfg.h>
#include <loongarch/prid.h>
#include <linux/api.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>


struct cpuinfo_loongarch_isa cpuinfo_isa = { 0 };

static struct cpuinfo_package package = { { 0 } };

static inline bool bitmask_all(uint32_t bitfield, uint32_t mask) {
	return (bitfield & mask) == mask;
}

static inline uint32_t min(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}

static inline int cmp(uint32_t a, uint32_t b) {
	return (a > b) - (a < b);
}

static bool cluster_siblings_parser(
	uint32_t processor, uint32_t siblings_start, uint32_t siblings_end,
	struct cpuinfo_loongarch_linux_processor* processors)
{
	processors[processor].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
	uint32_t package_leader_id = processors[processor].package_leader_id;

	for (uint32_t sibling = siblings_start; sibling < siblings_end; sibling++) {
		if (!bitmask_all(processors[sibling].flags, CPUINFO_LINUX_FLAG_VALID)) {
			cpuinfo_log_info("invalid processor %"PRIu32" reported as a sibling for processor %"PRIu32,
				sibling, processor);
			continue;
		}

		const uint32_t sibling_package_leader_id = processors[sibling].package_leader_id;
		if (sibling_package_leader_id < package_leader_id) {
			package_leader_id = sibling_package_leader_id;
		}

		processors[sibling].package_leader_id = package_leader_id;
		processors[sibling].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
	}

	processors[processor].package_leader_id = package_leader_id;

	return true;
}

static int cmp_loongarch_linux_processor(const void* ptr_a, const void* ptr_b) {
	const struct cpuinfo_loongarch_linux_processor* processor_a = (const struct cpuinfo_loongarch_linux_processor*) ptr_a;
	const struct cpuinfo_loongarch_linux_processor* processor_b = (const struct cpuinfo_loongarch_linux_processor*) ptr_b;

	/* Move usable processors towards the start of the array */
	const bool usable_a = bitmask_all(processor_a->flags, CPUINFO_LINUX_FLAG_VALID);
	const bool usable_b = bitmask_all(processor_b->flags, CPUINFO_LINUX_FLAG_VALID);
	if (usable_a != usable_b) {
		return (int) usable_b - (int) usable_a;
	}
	
	/* Compare based on processsor ID (i.e. processor 0 < processor 1) */
	const uint32_t pro_a = processor_a->system_processor_id;
	const uint32_t pro_b = processor_b->system_processor_id;
	
	return cmp(pro_a,pro_b);


}

static void try_set_prid_by_cpucfg(
	struct cpuinfo_loongarch_linux_processor *processors,
	uint32_t count)
{
	uint32_t prid;
	if (!cpucfg(CPUCFG_REG_PRID, &prid)) {
		return;
	}
	for (uint32_t i = 0; i < count; i++) {
		processors[i].prid = prid;
		processors[i].flags |= CPUINFO_LOONGARCH_LINUX_VALID_PRID;
	}
}

void cpuinfo_loongarch_linux_init(void) {
	struct cpuinfo_loongarch_linux_processor* loongarch_linux_processors = NULL;
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_uarch_info* uarchs = NULL;
	const struct cpuinfo_processor** linux_cpu_to_processor_map = NULL;
	const struct cpuinfo_core** linux_cpu_to_core_map = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	struct cpuinfo_cache* l3 = NULL;
	uint32_t* linux_cpu_to_uarch_index_map = NULL;

	const uint32_t max_processors_count = cpuinfo_linux_get_max_processors_count();
	cpuinfo_log_debug("system maximum processors count: %"PRIu32, max_processors_count);

	const uint32_t max_possible_processors_count = 1 +
		cpuinfo_linux_get_max_possible_processor(max_processors_count);
	cpuinfo_log_debug("maximum possible processors count: %"PRIu32, max_possible_processors_count);
	const uint32_t max_present_processors_count = 1 +
		cpuinfo_linux_get_max_present_processor(max_processors_count);
	cpuinfo_log_debug("maximum present processors count: %"PRIu32, max_present_processors_count);

	uint32_t valid_processor_mask = 0;
	uint32_t loongarch_linux_processors_count = max_processors_count;
	if (max_present_processors_count != 0) {
		loongarch_linux_processors_count = min(loongarch_linux_processors_count, max_present_processors_count);
		valid_processor_mask = CPUINFO_LINUX_FLAG_PRESENT;
	}
	if (max_possible_processors_count != 0) {
		loongarch_linux_processors_count = min(loongarch_linux_processors_count, max_possible_processors_count);
		valid_processor_mask |= CPUINFO_LINUX_FLAG_POSSIBLE;
	}
	if ((max_present_processors_count | max_possible_processors_count) == 0) {
		cpuinfo_log_error("failed to parse both lists of possible and present processors");
		return;
	}

	loongarch_linux_processors = calloc(loongarch_linux_processors_count, sizeof(struct cpuinfo_loongarch_linux_processor));
	if (loongarch_linux_processors == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" Loongarch logical processors",
			loongarch_linux_processors_count * sizeof(struct cpuinfo_loongarch_linux_processor),
			loongarch_linux_processors_count);
		return;
	}

	if (max_possible_processors_count) {
		cpuinfo_linux_detect_possible_processors(
			loongarch_linux_processors_count, &loongarch_linux_processors->flags,
			sizeof(struct cpuinfo_loongarch_linux_processor),
			CPUINFO_LINUX_FLAG_POSSIBLE);
	}

	if (max_present_processors_count) {
		cpuinfo_linux_detect_present_processors(
			loongarch_linux_processors_count, &loongarch_linux_processors->flags,
			sizeof(struct cpuinfo_loongarch_linux_processor),
			CPUINFO_LINUX_FLAG_PRESENT);
	}

	#if CPUINFO_ARCH_LOONGARCH64
	/* Populate ISA structure with hwcap information. */
	cpuinfo_loongarch64_linux_decode_isa_from_hwcap(&cpuinfo_isa);
	if (cpuinfo_isa.cpucfg) {
		try_set_prid_by_cpucfg(loongarch_linux_processors, loongarch_linux_processors_count);
	}
	#endif

	/* Populate processor information. */
	char proc_cpuinfo_hardware[CPUINFO_HARDWARE_VALUE_MAX];
	uint32_t valid_processors = 0;

	if (!cpuinfo_loongarch_linux_parse_proc_cpuinfo(
			proc_cpuinfo_hardware,
			loongarch_linux_processors_count,
			loongarch_linux_processors)) {
		cpuinfo_log_error("failed to parse processor information from /proc/cpuinfo");
		return;
	}
	
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, valid_processor_mask)) {
			loongarch_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_VALID;
		}
	}

	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		loongarch_linux_processors[i].system_processor_id = i;
		if (bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			valid_processors += 1;

			if (!(loongarch_linux_processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR)) {
				/*
				 * Processor is in possible and present lists, but not reported in /proc/cpuinfo.
				 * This is fairly common: high-index processors can be not reported if they are offline.
				 */
				cpuinfo_log_info("processor %"PRIu32" is not listed in /proc/cpuinfo", i);
			}

		} else {
			/* Processor reported in /proc/cpuinfo, but not in possible and/or present lists: log and ignore */
			if (!(loongarch_linux_processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR)) {
				cpuinfo_log_warning("invalid processor %"PRIu32" reported in /proc/cpuinfo", i);
			}
		}
	}

	const struct cpuinfo_loongarch_chipset chipset =
		cpuinfo_loongarch_linux_decode_chipset(proc_cpuinfo_hardware);

	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			if (cpuinfo_linux_get_processor_package_id(i, &loongarch_linux_processors[i].package_id)) {
				loongarch_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_ID;
			}
		}
	}

	/* Initialize topology group IDs */
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		loongarch_linux_processors[i].package_leader_id = i;
	}

	/* Propagate topology group IDs among siblings */
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}

		if (loongarch_linux_processors[i].flags & CPUINFO_LINUX_FLAG_PACKAGE_ID) {
			cpuinfo_linux_detect_core_siblings(
				loongarch_linux_processors_count, i,
				(cpuinfo_siblings_callback) cluster_siblings_parser,
				loongarch_linux_processors);
		}
	}

	/* Propagate all cluster IDs */
	uint32_t clustered_processors = 0;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID | CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER)) {
			clustered_processors += 1;

			const uint32_t package_leader_id = loongarch_linux_processors[i].package_leader_id;
			if (package_leader_id < i) {
				loongarch_linux_processors[i].package_leader_id = loongarch_linux_processors[package_leader_id].package_leader_id;
			}

			cpuinfo_log_debug("processor %"PRIu32" clustered with processor %"PRIu32" as inferred from system siblings lists",
				i, loongarch_linux_processors[i].package_leader_id);
		}
	}

	cpuinfo_loongarch_linux_count_cluster_processors(loongarch_linux_processors_count, loongarch_linux_processors);

	const uint32_t cluster_count = cpuinfo_loongarch_linux_detect_cluster_prid(
		&chipset,
		loongarch_linux_processors_count, valid_processors, loongarch_linux_processors);

	/* Initialize core vendor, uarch, and prid for every logical processor */
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			const uint32_t cluster_leader = loongarch_linux_processors[i].package_leader_id;
			if (cluster_leader == i) {
				/* Cluster leader: decode core vendor and uarch */
				cpuinfo_loongarch_decode_vendor_uarch(
				loongarch_linux_processors[cluster_leader].prid,
				&loongarch_linux_processors[cluster_leader].vendor,
				&loongarch_linux_processors[cluster_leader].uarch);
			} else {
				/* Cluster non-leader: copy vendor, uarch, and prid from cluster leader */
				loongarch_linux_processors[i].flags = loongarch_linux_processors[cluster_leader].flags;
				loongarch_linux_processors[i].prid = loongarch_linux_processors[cluster_leader].prid;
				loongarch_linux_processors[i].vendor = loongarch_linux_processors[cluster_leader].vendor;
				loongarch_linux_processors[i].uarch = loongarch_linux_processors[cluster_leader].uarch;
			}
		}
	}


	qsort(loongarch_linux_processors, loongarch_linux_processors_count,
		sizeof(struct cpuinfo_loongarch_linux_processor), cmp_loongarch_linux_processor);


	uint32_t uarchs_count = 0;
	enum cpuinfo_uarch last_uarch;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			if (uarchs_count == 0 || loongarch_linux_processors[i].uarch != last_uarch) {
				last_uarch = loongarch_linux_processors[i].uarch;
				uarchs_count += 1;
			}
			loongarch_linux_processors[i].uarch_index = uarchs_count - 1;
		}
	}

	/*
	 * Assumptions:
	 * - No SMP (i.e. each core supports only one hardware thread).
	 * - Level 1 instruction and data caches are private to the core clusters.
	 * - Level 2 and level 3 cache is shared between cores in the same cluster.
	 */
	cpuinfo_loongarch_chipset_to_string(&chipset, package.name);
	
	package.processor_count = valid_processors;
	package.core_count = valid_processors;
	package.cluster_count = cluster_count;

	processors = calloc(valid_processors, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
			valid_processors * sizeof(struct cpuinfo_processor), valid_processors);
		goto cleanup;
	}

	cores = calloc(valid_processors, sizeof(struct cpuinfo_core));
	if (cores == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" cores",
			valid_processors * sizeof(struct cpuinfo_core), valid_processors);
		goto cleanup;
	}

	clusters = calloc(cluster_count, sizeof(struct cpuinfo_cluster));
	if (clusters == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" core clusters",
			cluster_count * sizeof(struct cpuinfo_cluster), cluster_count);
		goto cleanup;
	}

	uarchs = calloc(uarchs_count, sizeof(struct cpuinfo_uarch_info));
	if (uarchs == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" microarchitectures",
			uarchs_count * sizeof(struct cpuinfo_uarch_info), uarchs_count);
		goto cleanup;
	}

	linux_cpu_to_processor_map = calloc(loongarch_linux_processors_count, sizeof(struct cpuinfo_processor*));
	if (linux_cpu_to_processor_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" logical processor mapping entries",
			loongarch_linux_processors_count * sizeof(struct cpuinfo_processor*), loongarch_linux_processors_count);
		goto cleanup;
	}

	linux_cpu_to_core_map = calloc(loongarch_linux_processors_count, sizeof(struct cpuinfo_core*));
	if (linux_cpu_to_core_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" core mapping entries",
			loongarch_linux_processors_count * sizeof(struct cpuinfo_core*), loongarch_linux_processors_count);
		goto cleanup;
	}

	if (uarchs_count > 1) {
		linux_cpu_to_uarch_index_map = calloc(loongarch_linux_processors_count, sizeof(uint32_t));
		if (linux_cpu_to_uarch_index_map == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" uarch index mapping entries",
				loongarch_linux_processors_count * sizeof(uint32_t), loongarch_linux_processors_count);
			goto cleanup;
		}
	}

	l1i = calloc(valid_processors, sizeof(struct cpuinfo_cache));
	if (l1i == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
			valid_processors * sizeof(struct cpuinfo_cache), valid_processors);
		goto cleanup;
	}

	l1d = calloc(valid_processors, sizeof(struct cpuinfo_cache));
	if (l1d == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
			valid_processors * sizeof(struct cpuinfo_cache), valid_processors);
		goto cleanup;
	}

	uint32_t uarchs_index = 0;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			if (uarchs_index == 0 || loongarch_linux_processors[i].uarch != last_uarch) {
				last_uarch = loongarch_linux_processors[i].uarch;
				uarchs[uarchs_index] = (struct cpuinfo_uarch_info) {
					.uarch = loongarch_linux_processors[i].uarch,
				};
				uarchs_index += 1;
			}
			uarchs[uarchs_index - 1].processor_count += 1;
			uarchs[uarchs_index - 1].core_count += 1;
		}
	}


	uint32_t l2_count = 0, l3_count = 0, big_l3_size = 0, cluster_id = UINT32_MAX;
	/* Indication whether L3 (if it exists) is shared between all cores */
	bool shared_l3 = true;
	/* Populate cache information structures in l1i, l1d */
	for (uint32_t i = 0; i < valid_processors; i++) {
		if (loongarch_linux_processors[i].package_leader_id == loongarch_linux_processors[i].system_processor_id) {
			cluster_id += 1;
			clusters[cluster_id] = (struct cpuinfo_cluster) {
				.processor_start = i,
				.processor_count = loongarch_linux_processors[i].package_processor_count,
				.core_start = i,
				.core_count = loongarch_linux_processors[i].package_processor_count,
				.cluster_id = cluster_id,
				.package = &package,
				.vendor = loongarch_linux_processors[i].vendor,
				.uarch = loongarch_linux_processors[i].uarch,
			};
		}

		processors[i].smt_id = 0;
		processors[i].core = cores + i;
		processors[i].cluster = clusters + cluster_id;
		processors[i].package = &package;
		processors[i].linux_id = (int) loongarch_linux_processors[i].system_processor_id;
		processors[i].cache.l1i = l1i + i;
		processors[i].cache.l1d = l1d + i;
		linux_cpu_to_processor_map[loongarch_linux_processors[i].system_processor_id] = &processors[i];

		cores[i].processor_start = i;
		cores[i].processor_count = 1;
		cores[i].core_id = i;
		cores[i].cluster = clusters + cluster_id;
		cores[i].package = &package;
		cores[i].vendor = loongarch_linux_processors[i].vendor;
		cores[i].uarch = loongarch_linux_processors[i].uarch;
		cores[i].prid = loongarch_linux_processors[i].prid;
		linux_cpu_to_core_map[loongarch_linux_processors[i].system_processor_id] = &cores[i];

		if (linux_cpu_to_uarch_index_map != NULL) {
			linux_cpu_to_uarch_index_map[loongarch_linux_processors[i].system_processor_id] =
				loongarch_linux_processors[i].uarch_index;
		}

		struct cpuinfo_cache temp_l2 = { 0 }, temp_l3 = { 0 };
		cpuinfo_loongarch_decode_cache(
			loongarch_linux_processors[i].uarch,
			loongarch_linux_processors[i].package_processor_count,
			loongarch_linux_processors[i].architecture_version,
			&l1i[i], &l1d[i], &temp_l2, &temp_l3);
		l1i[i].processor_start = l1d[i].processor_start = i;
		l1i[i].processor_count = l1d[i].processor_count = 1;
		

		if (temp_l3.size != 0) {
			/*
			 * Assumptions:
			 * - L2 is private to each core
			 * - L3 is shared by cores in the same cluster
			 * - If cores in different clusters report the same L3, it is shared between all cores.
			 */
			l2_count += 1;
			if (loongarch_linux_processors[i].package_leader_id == loongarch_linux_processors[i].system_processor_id) {
				if (cluster_id == 0) {
					big_l3_size = temp_l3.size;
					l3_count = 1;
				} else if (temp_l3.size != big_l3_size) {
					/* If some cores have different L3 size, L3 is not shared between all cores */
					shared_l3 = false;
					l3_count += 1;
				}
			}
		} else {
			/* If some cores don't have L3 cache, L3 is not shared between all cores */
			shared_l3 = false;
			if (temp_l2.size != 0) {
				/* Assume L2 is shared by cores in the same cluster */
				if (loongarch_linux_processors[i].package_leader_id == loongarch_linux_processors[i].system_processor_id) {
					l2_count += 1;
				}
			}
		}
	}

	if (l2_count != 0) {
		l2 = calloc(l2_count, sizeof(struct cpuinfo_cache));
		if (l2 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2_count * sizeof(struct cpuinfo_cache), l2_count);
			goto cleanup;
		}

		if (l3_count != 0) {
			l3 = calloc(l3_count, sizeof(struct cpuinfo_cache));
			if (l3 == NULL) {
				cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L3 caches",
					l3_count * sizeof(struct cpuinfo_cache), l3_count);
				goto cleanup;
			}
		}
	}

	cluster_id = UINT32_MAX;
	uint32_t l2_index = UINT32_MAX, l3_index = UINT32_MAX;
	for (uint32_t i = 0; i < valid_processors; i++) {
		if (loongarch_linux_processors[i].package_leader_id == loongarch_linux_processors[i].system_processor_id) {
			cluster_id++;
		}

		struct cpuinfo_cache dummy_l1i, dummy_l1d, temp_l2 = { 0 }, temp_l3 = { 0 };
		cpuinfo_loongarch_decode_cache(
			loongarch_linux_processors[i].uarch,
			loongarch_linux_processors[i].package_processor_count,
			loongarch_linux_processors[i].architecture_version,
			&dummy_l1i, &dummy_l1d, &temp_l2, &temp_l3);

		if (temp_l3.size != 0) {
			/*
			 * Assumptions:
			 * - L2 is private to each core
			 * - L3 is shared by cores in the same cluster
			 * - If cores in different clusters report the same L3, it is shared between all cores.
			 */
			l2_index += 1;
			l2[l2_index] = (struct cpuinfo_cache) {
				.size            = temp_l2.size,
				.associativity   = temp_l2.associativity,
				.sets            = temp_l2.sets,
				.partitions      = 1,
				.line_size       = temp_l2.line_size,
				.flags           = temp_l2.flags,
				.processor_start = i,
				.processor_count = 1,
			};
			processors[i].cache.l2 = l2 + l2_index;
			if (loongarch_linux_processors[i].package_leader_id == loongarch_linux_processors[i].system_processor_id) {
				l3_index += 1;
				if (l3_index < l3_count) {
					l3[l3_index] = (struct cpuinfo_cache) {
						.size            = temp_l3.size,
						.associativity   = temp_l3.associativity,
						.sets            = temp_l3.sets,
						.partitions      = 1,
						.line_size       = temp_l3.line_size,
						.flags           = temp_l3.flags,
						.processor_start = i,
						.processor_count =
							shared_l3 ? valid_processors : loongarch_linux_processors[i].package_processor_count,
					};
				}
			}
			if (shared_l3) {
				processors[i].cache.l3 = l3;
			} else if (l3_index < l3_count) {
				processors[i].cache.l3 = l3 + l3_index;
			}
		} else if (temp_l2.size != 0) {
			/* Assume L2 is shared by cores in the same cluster */
			if (loongarch_linux_processors[i].package_leader_id == loongarch_linux_processors[i].system_processor_id) {
				l2_index += 1;
				l2[l2_index] = (struct cpuinfo_cache) {
					.size            = temp_l2.size,
					.associativity   = temp_l2.associativity,
					.sets            = temp_l2.sets,
					.partitions      = 1,
					.line_size       = temp_l2.line_size,
					.flags           = temp_l2.flags,
					.processor_start = i,
					.processor_count = loongarch_linux_processors[i].package_processor_count,
				};
			}
			processors[i].cache.l2 = l2 + l2_index;
		}
	}

	/* Commit */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
	cpuinfo_clusters = clusters;
	cpuinfo_packages = &package;
	cpuinfo_uarchs = uarchs;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;
	cpuinfo_cache[cpuinfo_cache_level_3]  = l3;

	cpuinfo_processors_count = valid_processors;
	cpuinfo_cores_count = valid_processors;
	cpuinfo_clusters_count = cluster_count;
	cpuinfo_packages_count = 1;
	cpuinfo_uarchs_count = uarchs_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = valid_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = valid_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2_count;
	cpuinfo_cache_count[cpuinfo_cache_level_3]  = l3_count;
	cpuinfo_max_cache_size = cpuinfo_loongarch_compute_max_cache_size(&processors[0]);

	cpuinfo_linux_cpu_max = loongarch_linux_processors_count;
	cpuinfo_linux_cpu_to_processor_map = linux_cpu_to_processor_map;
	cpuinfo_linux_cpu_to_core_map = linux_cpu_to_core_map;
	cpuinfo_linux_cpu_to_uarch_index_map = linux_cpu_to_uarch_index_map;

	__sync_synchronize();
	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;
	clusters = NULL;
	uarchs = NULL;
	l1i = l1d = l2 = l3 = NULL;
	linux_cpu_to_processor_map = NULL;
	linux_cpu_to_core_map = NULL;
	linux_cpu_to_uarch_index_map = NULL;

cleanup:
	free(loongarch_linux_processors);
	free(processors);
	free(cores);
	free(clusters);
	free(uarchs);
	free(l1i);
	free(l1d);
	free(l2);
	free(l3);
	free(linux_cpu_to_processor_map);
	free(linux_cpu_to_core_map);
	free(linux_cpu_to_uarch_index_map);
}
