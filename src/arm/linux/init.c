#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo.h>
#include <arm/linux/api.h>
#if defined(__ANDROID__)
	#include <arm/android/api.h>
#endif
#include <arm/api.h>
#include <arm/midr.h>
#include <linux/api.h>
#include <api.h>
#include <log.h>


struct cpuinfo_arm_isa cpuinfo_isa = { 0 };

static struct cpuinfo_package package = { 0 };

static inline bool bitmask_all(uint32_t bitfield, uint32_t mask) {
	return (bitfield & mask) == mask;
}

static inline uint32_t min(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}

static inline uint32_t max(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}

static inline int cmp(uint32_t a, uint32_t b) {
	return (a > b) - (a < b);
}

static bool cluster_siblings_parser(
	uint32_t processor, uint32_t siblings_start, uint32_t siblings_end,
	struct cpuinfo_arm_linux_processor* processors)
{
	processors[processor].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
	uint32_t package_leader_id = processors[processor].package_leader_id;

	for (uint32_t sibling = siblings_start; sibling < siblings_end; sibling++) {
		if (!bitmask_all(processors[sibling].flags, CPUINFO_LINUX_MASK_USABLE)) {
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

static int cmp_arm_linux_processor(const void* ptr_a, const void* ptr_b) {
	const struct cpuinfo_arm_linux_processor* processor_a = (const struct cpuinfo_arm_linux_processor*) ptr_a;
	const struct cpuinfo_arm_linux_processor* processor_b = (const struct cpuinfo_arm_linux_processor*) ptr_b;

	/* Move usable processors towards the start of the array */
	const bool usable_a = bitmask_all(processor_a->flags, CPUINFO_LINUX_MASK_USABLE);
	const bool usable_b = bitmask_all(processor_b->flags, CPUINFO_LINUX_MASK_USABLE);
	if (usable_a != usable_b) {
		return (int) usable_b - (int) usable_a;
	}

	/* Compare based on core type (e.g. Cortex-A57 < Cortex-A53) */
	const uint32_t midr_a = processor_a->midr;
	const uint32_t midr_b = processor_b->midr;
	if (midr_a != midr_b) {
		const uint32_t score_a = midr_score_core(midr_a);
		const uint32_t score_b = midr_score_core(midr_b);
		if (score_a != score_b) {
			return score_a > score_b ? -1 : 1;
		}
	}

	/* Compare based on core frequency (e.g. 2.0 GHz < 1.2 GHz) */
	const uint32_t frequency_a = processor_a->max_frequency;
	const uint32_t frequency_b = processor_b->max_frequency;
	if (frequency_a != frequency_b) {
		return frequency_a > frequency_b ? -1 : 1;
	}

	/* Compare based on cluster leader id (i.e. cluster 1 < cluster 0) */
	const uint32_t cluster_a = processor_a->package_leader_id;
	const uint32_t cluster_b = processor_b->package_leader_id;
	if (cluster_a != cluster_b) {
		return cluster_a > cluster_b ? -1 : 1;
	}

	/* Compare based on system processor id (i.e. processor 0 < processor 1) */
	const uint32_t id_a = processor_a->system_processor_id;
	const uint32_t id_b = processor_b->system_processor_id;
	return cmp(id_a, id_b);
}

void cpuinfo_arm_linux_init(void) {
	struct cpuinfo_arm_linux_processor* arm_linux_processors = NULL;
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;

	const uint32_t max_processors_count = cpuinfo_linux_get_max_processors_count();
	cpuinfo_log_debug("system maximum processors count: %"PRIu32, max_processors_count);

	const uint32_t max_possible_processors_count = 1 +
		cpuinfo_linux_get_max_possible_processor(max_processors_count);
	cpuinfo_log_debug("maximum possible processors count: %"PRIu32, max_possible_processors_count);
	const uint32_t max_present_processors_count = 1 +
		cpuinfo_linux_get_max_possible_processor(max_processors_count);
	cpuinfo_log_debug("maximum present processors count: %"PRIu32, max_present_processors_count);

	const uint32_t arm_linux_processors_count = min(max_possible_processors_count, max_present_processors_count);
	arm_linux_processors = calloc(arm_linux_processors_count, sizeof(struct cpuinfo_arm_linux_processor));
	if (arm_linux_processors == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" ARM logical processors",
			arm_linux_processors_count * sizeof(struct cpuinfo_arm_linux_processor),
			arm_linux_processors_count);
		return;
	}

	cpuinfo_linux_detect_possible_processors(
		arm_linux_processors_count, &arm_linux_processors->flags,
		sizeof(struct cpuinfo_arm_linux_processor),
		CPUINFO_LINUX_FLAG_POSSIBLE);

	cpuinfo_linux_detect_present_processors(
		arm_linux_processors_count, &arm_linux_processors->flags,
		sizeof(struct cpuinfo_arm_linux_processor),
		CPUINFO_LINUX_FLAG_PRESENT);

#if defined(__ANDROID__)
	struct cpuinfo_android_properties android_properties;
	cpuinfo_arm_android_parse_properties(&android_properties);
#else
	char proc_cpuinfo_hardware[CPUINFO_HARDWARE_VALUE_MAX] = { 0 };
#endif

	if (!cpuinfo_arm_linux_parse_proc_cpuinfo(
#if defined(__ANDROID__)
			android_properties.proc_cpuinfo_hardware,
#else
			proc_cpuinfo_hardware,
#endif
			arm_linux_processors_count,
			arm_linux_processors)) {
		cpuinfo_log_error("failed to parse processor information from /proc/cpuinfo");
		return;
	}

	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			cpuinfo_log_debug("parsed processor %"PRIu32" MIDR 0x%08"PRIx32,
				i, arm_linux_processors[i].midr);
		}
	}

	uint32_t usable_processors = 0;
	uint32_t known_processors = 0;
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		arm_linux_processors[i].system_processor_id = i;
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			arm_linux_processors[i].processor_id = usable_processors;
			usable_processors += 1;

			if (!(arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_PROCESSOR)) {
				/*
				 * Processor is in possible and present lists, but not reported in /proc/cpuinfo.
				 * This is fairly common: high-index processors can be not reported if they are offline.
				 */
				cpuinfo_log_info("processor %"PRIu32" is not listed in /proc/cpuinfo", i);
			}

			if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_ARM_LINUX_VALID_INFO)) {
				if (known_processors == 0) {
					/*
					 * This is the first processor for which we have complete information.
					 * Use it to detect instruction set architecture.
					 * If there are several cores with different microarchitecture, we expect
					 * Linux kernel to report the same ISA extensions for each of them.
					 */
					#if CPUINFO_ARCH_ARM
						cpuinfo_arm_linux_decode_isa_from_proc_cpuinfo(
							&arm_linux_processors[i], &cpuinfo_isa);
					#elif CPUINFO_ARCH_ARM64
						cpuinfo_arm64_linux_decode_isa_from_proc_cpuinfo(
							&arm_linux_processors[i], &cpuinfo_isa);
					#endif
				}

				known_processors += 1;
			}
		} else {
			/* Processor reported in /proc/cpuinfo, but not in possible and/or present lists: log and ignore */
			if (!(arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_PROCESSOR)) {
				cpuinfo_log_warning("invalid processor %"PRIu32" reported in /proc/cpuinfo", i);
			}
		}
	}

	/* Detect min/max frequency and package ID */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			const uint32_t max_frequency = cpuinfo_linux_get_processor_max_frequency(i);
			if (max_frequency != 0) {
				arm_linux_processors[i].max_frequency = max_frequency;
				arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_MAX_FREQUENCY;
			}

			const uint32_t min_frequency = cpuinfo_linux_get_processor_min_frequency(i);
			if (min_frequency != 0) {
				arm_linux_processors[i].min_frequency = min_frequency;
				arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_MIN_FREQUENCY;
			}

			if (cpuinfo_linux_get_processor_package_id(i, &arm_linux_processors[i].package_id)) {
				arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_ID;
			}
		}
	}

	/* Initialize topology group IDs */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		arm_linux_processors[i].package_leader_id = i;
	}

	/* Propagate topology group IDs among siblings */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (!bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			continue;
		}

		if (arm_linux_processors[i].flags & CPUINFO_LINUX_FLAG_PACKAGE_ID) {
			cpuinfo_linux_detect_core_siblings(
				arm_linux_processors_count, i,
				(cpuinfo_siblings_callback) cluster_siblings_parser,
				arm_linux_processors);
		}
	}

	/* Propagate all cluster IDs */
	uint32_t clustered_processors = 0;
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE | CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER)) {
			clustered_processors += 1;

			const uint32_t package_leader_id = arm_linux_processors[i].package_leader_id;
			if (package_leader_id < i) {
				arm_linux_processors[i].package_leader_id = arm_linux_processors[package_leader_id].package_leader_id;
			}

			cpuinfo_log_debug("processor %"PRIu32" clustered with processor %"PRIu32" as inferred from system siblings lists",
				i, arm_linux_processors[i].package_leader_id);
		}
	}

	if (clustered_processors != usable_processors) {
		/*
		 * Topology information about some or all logical processors may be unavailable, for the following reasons:
		 * - Linux kernel is too old, or configured without support for topology information in sysfs.
		 * - Core is offline, and Linux kernel is configured to not report topology for offline cores.
		 *
		 * In this case, we assign processors to clusters using two methods:
		 * - Try heuristic cluster configurations (e.g. 6-core SoC usually has 4+2 big.LITTLE configuration).
		 * - If heuristic failed, assign processors to core clusters in a sequential scan.
		 */
		if (!cpuinfo_arm_linux_detect_core_clusters_by_heuristic(usable_processors, arm_linux_processors_count, arm_linux_processors)) {
			cpuinfo_arm_linux_detect_core_clusters_by_sequential_scan(arm_linux_processors_count, arm_linux_processors);
		}
	}

	cpuinfo_arm_linux_count_cluster_processors(arm_linux_processors_count, arm_linux_processors);

#if defined(__ANDROID__)
	const struct cpuinfo_arm_chipset chipset =
		cpuinfo_arm_android_decode_chipset(&android_properties, usable_processors, 0);
#endif

	const uint32_t cluster_count = cpuinfo_arm_linux_detect_cluster_midr(
#if defined(__ANDROID__)
		&chipset,
#endif
		arm_linux_processors_count, usable_processors, arm_linux_processors);

	/* Initialize core vendor, uarch, MIDR, and frequency for every logical processor */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			const uint32_t cluster_leader = arm_linux_processors[i].package_leader_id;
			if (cluster_leader == i) {
				/* Cluster leader: decode core vendor and uarch */
				cpuinfo_arm_decode_vendor_uarch(
				arm_linux_processors[cluster_leader].midr,
#if CPUINFO_ARCH_ARM
				!!(arm_linux_processors[cluster_leader].features & CPUINFO_ARM_LINUX_FEATURE_VFPV4),
#endif
				&arm_linux_processors[cluster_leader].vendor,
				&arm_linux_processors[cluster_leader].uarch);
			} else {
				/* Cluster non-leader: copy vendor, uarch, MIDR, and frequency from cluster leader */
				arm_linux_processors[i].flags |= arm_linux_processors[cluster_leader].flags &
					(CPUINFO_ARM_LINUX_VALID_MIDR | CPUINFO_LINUX_FLAG_MAX_FREQUENCY);
				arm_linux_processors[i].midr = arm_linux_processors[cluster_leader].midr;
				arm_linux_processors[i].vendor = arm_linux_processors[cluster_leader].vendor;
				arm_linux_processors[i].uarch = arm_linux_processors[cluster_leader].uarch;
				arm_linux_processors[i].max_frequency = arm_linux_processors[cluster_leader].max_frequency;
			}
		}
	}

	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			cpuinfo_log_debug("post-analysis processor %"PRIu32": MIDR %08"PRIx32" frequency %"PRIu32,
				i, arm_linux_processors[i].midr, arm_linux_processors[i].max_frequency);
		}
	}

	qsort(arm_linux_processors, arm_linux_processors_count,
		sizeof(struct cpuinfo_arm_linux_processor), cmp_arm_linux_processor);

	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			cpuinfo_log_debug("post-sort processor %"PRIu32": system id %"PRIu32" MIDR %08"PRIx32" frequency %"PRIu32,
				i, arm_linux_processors[i].system_processor_id, arm_linux_processors[i].midr, arm_linux_processors[i].max_frequency);
		}
	}

	processors = calloc(usable_processors, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
			usable_processors * sizeof(struct cpuinfo_processor), usable_processors);
		goto cleanup;
	}

	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		processors[i].vendor = arm_linux_processors[i].vendor;
		processors[i].uarch = arm_linux_processors[i].uarch;
		processors[i].linux_id = (int) arm_linux_processors[i].system_processor_id;
		processors[i].topology = (struct cpuinfo_topology) {
			.thread_id = 0,
			.core_id = arm_linux_processors[i].system_processor_id,
			.package_id = 0,
		};
	}

	/*
	 * Assumptions:
	 * - No SMP (i.e. each core supports only one hardware thread).
	 * - Level 1 instruction and data caches are private to the core clusters.
	 * - Level 2 cache is shared between cores in the same cluster.
	 */
#if defined(__ANDROID__)
	cpuinfo_arm_chipset_to_string(&chipset, package.name);
#endif
	package.processor_count = package.core_count = usable_processors;

	l1i = calloc(usable_processors, sizeof(struct cpuinfo_cache));
	if (l1i == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
			usable_processors * sizeof(struct cpuinfo_cache), usable_processors);
		goto cleanup;
	}

	l1d = calloc(usable_processors, sizeof(struct cpuinfo_cache));
	if (l1d == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
			usable_processors * sizeof(struct cpuinfo_cache), usable_processors);
		goto cleanup;
	}

	uint32_t l2_count = cluster_count;
	l2 = calloc(l2_count, sizeof(struct cpuinfo_cache));
	if (l2 == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
			l2_count * sizeof(struct cpuinfo_cache), l2_count);
		goto cleanup;
	}

	/* Populate cache infromation structures in l1i, l1d, and l2 */
	struct cpuinfo_cache shared_l2;	
	uint32_t l2_index = 0;
	for (uint32_t i = 0; i < usable_processors; i++) {
		cpuinfo_arm_decode_cache(
			processors[i].uarch,
			arm_linux_processors[i].package_processor_count,
			arm_linux_processors[i].midr,
#if defined(__ANDROID__)
			&chipset,
#endif
			arm_linux_processors[i].architecture_version,
			&l1i[i], &l1d[i], &shared_l2);
		l1i[i].processor_start = l1d[i].processor_start = i;
		l1i[i].processor_count = l1d[i].processor_count = 1;
		#if CPUINFO_ARCH_ARM
			/* L1I reported in /proc/cpuinfo overrides defaults */
			if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_ARM_LINUX_VALID_ICACHE)) {
				l1i[i] = (struct cpuinfo_cache) {
					.size = arm_linux_processors[i].proc_cpuinfo_cache.i_size,
					.associativity = arm_linux_processors[i].proc_cpuinfo_cache.i_assoc,
					.sets = arm_linux_processors[i].proc_cpuinfo_cache.i_sets,
					.partitions = 1,
					.line_size = arm_linux_processors[i].proc_cpuinfo_cache.i_line_length
				};
			}
			/* L1D reported in /proc/cpuinfo overrides defaults */
			if (bitmask_all(arm_linux_processors[i].flags, CPUINFO_ARM_LINUX_VALID_ICACHE)) {
				l1d[i] = (struct cpuinfo_cache) {
					.size = arm_linux_processors[i].proc_cpuinfo_cache.d_size,
					.associativity = arm_linux_processors[i].proc_cpuinfo_cache.d_assoc,
					.sets = arm_linux_processors[i].proc_cpuinfo_cache.d_sets,
					.partitions = 1,
					.line_size = arm_linux_processors[i].proc_cpuinfo_cache.d_line_length
				};
			}
		#endif
		if (arm_linux_processors[i].package_leader_id == arm_linux_processors[i].system_processor_id) {
			shared_l2.processor_start = i;
			shared_l2.processor_count = arm_linux_processors[i].package_processor_count;
			l2[l2_index++] = shared_l2;
		}
	}

	if (cluster_count == 1 && l2[0].size == 0) {
		/* CPU without L2 cache */
		free(l2);
		l2 = NULL;
		l2_count = 0;
	}

	/* Commit */
	cpuinfo_processors = processors;
	cpuinfo_packages = &package;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;

	cpuinfo_processors_count = usable_processors;
	cpuinfo_packages_count = 1;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = usable_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = usable_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2_count;

	processors = NULL;
	l1i = l1d = l2 = NULL;

cleanup:
	free(arm_linux_processors);
	free(processors);
	free(l1i);
	free(l1d);
	free(l2);
}
