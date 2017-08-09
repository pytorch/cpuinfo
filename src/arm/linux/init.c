#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo.h>
#include <arm/linux/api.h>
#include <arm/api.h>
#include <arm/midr.h>
#include <linux/api.h>
#include <api.h>
#include <log.h>


struct cpuinfo_arm_isa cpuinfo_isa = { 0 };


static inline uint32_t min(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}

static inline uint32_t max(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}

static inline int cmp(uint32_t a, uint32_t b) {
	return (a > b) - (a < b);
}

static int cmp_x86_processor_by_apic_id(const void* ptr_a, const void* ptr_b) {
	const struct cpuinfo_arm_linux_processor* processor_a = (const struct cpuinfo_arm_linux_processor*) ptr_a;
	const struct cpuinfo_arm_linux_processor* processor_b = (const struct cpuinfo_arm_linux_processor*) ptr_b;

	/* Move usable processors towards the start of the array */
	const bool usable_a = (processor_a->flags & CPUINFO_LINUX_MASK_USABLE) == CPUINFO_LINUX_MASK_USABLE;
	const bool usable_b = (processor_b->flags & CPUINFO_LINUX_MASK_USABLE) == CPUINFO_LINUX_MASK_USABLE;
	if (usable_a != usable_b) {
		return (int) usable_b - (int) usable_a;
	}

	/* Compare based on core type (e.g. Cortex-A57 < Cortex-A53) */
	const uint32_t midr_a = processor_a->midr;
	const uint32_t midr_b = processor_b->midr;
	if (midr_a != midr_b) {
		if (midr_is_big_core(midr_a) || midr_is_little_core(midr_b)) {
			return -1;
		} else if (midr_is_big_core(midr_b) || midr_is_little_core(midr_a)) {
			return 1;
		}
	}

	/* Compare based on core frequency (e.g. 2.0 GHz < 1.2 GHz) */
	const uint32_t frequency_a = processor_a->max_frequency;
	const uint32_t frequency_b = processor_b->max_frequency;
	if (frequency_a != frequency_b) {
		return frequency_a > frequency_b ? -1 : 1;
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

	if (!cpuinfo_arm_linux_parse_proc_cpuinfo(arm_linux_processors_count, arm_linux_processors)) {
		cpuinfo_log_error("failed to parse processor information from /proc/cpuinfo");
		return;
	}

	uint32_t usable_processors = 0;
	uint32_t known_processors = 0;
	uint32_t last_reported_processor = 0;
	uint32_t last_reported_midr = 0;
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		arm_linux_processors[i].system_processor_id = i;
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) == CPUINFO_LINUX_MASK_USABLE) {
			arm_linux_processors[i].processor_id = usable_processors;
			usable_processors += 1;

			if (arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_PROCESSOR) {
				last_reported_processor = i;
			} else {
				/*
				 * Processor is in possible and present lists, but not reported in /proc/cpuinfo.
				 * This is fairly common: high-index processors can be not reported if they are offline.
				 */
				cpuinfo_log_info("processor %"PRIu32" is not listed in /proc/cpuinfo", i);
			}

			if ((arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_INFO) == CPUINFO_ARM_LINUX_VALID_INFO) {
				last_reported_midr = i;
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

	/* Detect min/max frequency, core ID, and package ID */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) == CPUINFO_LINUX_MASK_USABLE) {
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

			if (cpuinfo_linux_get_processor_core_id(i, &arm_linux_processors[i].core_id)) {
				arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_CORE_ID;
			}

			if (cpuinfo_linux_get_processor_package_id(i, &arm_linux_processors[i].package_id)) {
				arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_ID;
			}
		}
	}

	/* Initialize topology group IDs */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		arm_linux_processors[i].core_group_min = arm_linux_processors[i].core_group_max = i;
		arm_linux_processors[i].package_group_min = arm_linux_processors[i].package_group_max = i;
	}

	/* Propagate topology group IDs among siblings */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
			continue;
		}

		cpuinfo_linux_detect_core_siblings(
			arm_linux_processors_count,
			i,
			&arm_linux_processors->flags,
			&arm_linux_processors->package_id,
			&arm_linux_processors->package_group_min,
			&arm_linux_processors->package_group_max,
			sizeof(struct cpuinfo_arm_linux_processor));
		cpuinfo_linux_detect_thread_siblings(
			arm_linux_processors_count,
			i,
			&arm_linux_processors->flags,
			&arm_linux_processors->core_id,
			&arm_linux_processors->core_group_min,
			&arm_linux_processors->core_group_max,
			sizeof(struct cpuinfo_arm_linux_processor));
	}

	/*
	 * Topology information about some or all logical processors may be unavailable, for the following reasons:
	 * - Linux kernel is too old, or configured without support for topology information in sysfs.
	 * - Core is offline, and Linux kernel is configured to not report topology for offline cores.
	 *
	 * In these cases, we use a fall-back mechanism for topology detection, based on the assumption that equivalent
	 * cores belong to the same cluster:
	 * - Cores with the same min/max frequency and microarchitecture are assumed to belong to the same cluster.
	 * - If min or max frequency is not known for any of the cores, but microarchitecture for both cores is the same,
	 *   and different from Cortex-A53, both cores are assumed to belong to the same cluster. Cortex-A53 is the only
	 *   microarchitecture, which is simultaneously used in multiple clusters in the same SoCs, e.g. Qualcomm
	 *   Snapdragon 615 combines 4 "big" Cortex-A53 cores + 4 "LITTLE" Cortex-A53 cores, and MediaTek Helio X20
	 *   combines 2 "max" Cortex-A72 cores + 4 "med" Cortex-A53 cores + 4 "min" Cortex-A53 cores.
	 * - If microarchitecture is not known, but min/max frequency are the same for two cores, assume both cores
	 *   belong to the same cluster.
	 */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
			continue;
		}

		if (arm_linux_processors[i].flags & CPUINFO_LINUX_FLAG_PACKAGE_ID) {
			continue;
		}

		for (uint32_t j = 0; j < arm_linux_processors_count; j++) {
			if (i == j) {
				continue;
			}

			if ((arm_linux_processors[j].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
				/* Logical processor is not possible or not present */
				continue;
			}

			if (arm_linux_processors[j].flags & CPUINFO_LINUX_FLAG_PACKAGE_ID) {
				/* Cluster for this processor was already parsed from sysfs */
				continue;
			}

			if (cpuinfo_arm_linux_processor_equals(&arm_linux_processors[i], &arm_linux_processors[j])) {
				cpuinfo_log_info(
					"processors %"PRIu32" and %"PRIu32" are assigned to the same cluster based on similarity", i, j);

				arm_linux_processors[i].package_group_min = arm_linux_processors[j].package_group_min =
					min(arm_linux_processors[i].package_group_min, arm_linux_processors[j].package_group_min);
				arm_linux_processors[i].package_group_max = arm_linux_processors[j].package_group_max =
					max(arm_linux_processors[i].package_group_max, arm_linux_processors[j].package_group_max);
				arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
				arm_linux_processors[j].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
			}
		}
	}

	/*
	 * It may happen that neither of sysfs topology information, min/max frequencies, or microarchitecture
	 * is known for some or all cores. This can happen for the following reasons:
	 * - Kernel is configured without support for sysfs cpufreq and topology information, and reports
	 *   detailed information only for one of the cores listed in /proc/cpuinfo
	 * - Some of the cores are offline, and Linux kernel is configured to report information only about
	 *   online cores.
	 *
	 * In this case, it is generally impossible to reconstruct topology information, and we use a heuristic:
	 * each core which wasn't assigned to any cluster yet, is assumed to belong to the same cluster as
	 * the preceeding core for which no sysfs information is available.
	 */
	uint32_t cluster_processor_id = 0;
	bool last_processor_has_sysfs_topology = false;
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
			continue;
		}

		if (arm_linux_processors[i].flags & CPUINFO_LINUX_FLAG_PACKAGE_ID) {
			/* sysfs topology information is available for this processor */
			last_processor_has_sysfs_topology = true;
		} else {
			if (last_processor_has_sysfs_topology) {
				/*
				 * Subsequent processors unassigned to any cluster will be added to the cluster of this
				 * processor. Note that if this processor itself is not assigned to any cluster,
				 * it will start a new cluster of processors.
				 */
				cluster_processor_id = i;
			}
			last_processor_has_sysfs_topology = false;
		}

		if (!(arm_linux_processors[i].flags & CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER)) {
			// TODO: check that processors are not the same
			if (cluster_processor_id == i) {
				cpuinfo_log_info("processor %"PRIu32" is assumed to belong to a new cluster", i);
			} else {
				cpuinfo_log_info("processor %"PRIu32" is assumed to belong to the cluster of processor %"PRIu32,
					i, cluster_processor_id);
				arm_linux_processors[i].package_group_min = arm_linux_processors[cluster_processor_id].package_group_min;
				arm_linux_processors[cluster_processor_id].package_group_max =
					arm_linux_processors[i].package_group_max =
						max(i, arm_linux_processors[cluster_processor_id].package_group_max);
			}
			arm_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
		}
	}

	/*
	 * Run Shiloach-Vishkin (well, almost) connected components algorithm
	 */
	uint32_t update;
	do {
		update = 0;
		for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
			if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
				continue;
			}

			const uint32_t group_max_processor_id = arm_linux_processors[i].package_group_max;
			const uint32_t group_min_processor_id = arm_linux_processors[i].package_group_min;

			const uint32_t group_max_processor_group_max = arm_linux_processors[group_max_processor_id].package_group_max;
			const uint32_t group_max_processor_group_min = arm_linux_processors[group_max_processor_id].package_group_min;
			const uint32_t group_min_processor_group_max = arm_linux_processors[group_min_processor_id].package_group_max;
			const uint32_t group_min_processor_group_min = arm_linux_processors[group_min_processor_id].package_group_min;

			const uint32_t new_group_max_processor_id = max(group_max_processor_group_max, group_min_processor_group_max);
			const uint32_t new_group_min_processor_id = min(group_min_processor_group_min, group_max_processor_group_min);

			arm_linux_processors[i].package_group_max =
				arm_linux_processors[group_max_processor_id].package_group_max =
					arm_linux_processors[group_min_processor_id].package_group_max =
						new_group_max_processor_id;
			arm_linux_processors[i].package_group_min =
				arm_linux_processors[group_max_processor_id].package_group_min =
					arm_linux_processors[group_min_processor_id].package_group_min =
						new_group_min_processor_id;

			update |= (group_max_processor_id ^ new_group_max_processor_id) | (group_min_processor_id ^ new_group_min_processor_id) |
				(group_max_processor_group_max ^ new_group_max_processor_id) | (group_max_processor_group_min ^ new_group_min_processor_id) |
				(group_min_processor_group_max ^ new_group_max_processor_id) | (group_min_processor_group_min ^ new_group_min_processor_id);
		}
	} while (update != 0);

	uint32_t cluster_count = 0;
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
			continue;
		}

		if (arm_linux_processors[i].package_group_min == i) {
			cluster_count += 1;
		}
	}
	cpuinfo_log_info("detected %"PRIu32" core clusters", cluster_count);

	/*
	 * Two relations between reported /proc/cpuinfo information, and cores is possible:
	 * - /proc/cpuinfo reports information for all or some of the cores below the corresponding
	 *   "processor : <number>" lines. Information on offline cores may be missing.
	 * - /proc/cpuinfo reports information only once, after all "processor : <number>" lines.
	 *   The reported information may relate to processor #0 or to the processor which
	 *   executed the system calls to read /proc/cpuinfo. It is also indistinguishable
	 *   from /proc/cpuinfo reporting information only for the last core (e.g. if all other
	 *   cores are offline).
	 *
	 * We detect the second case by checking if /proc/cpuinfo contains valid MIDR only for one,
	 * last reported, processor. Note, that the last reported core may be not the last
	 * present+possible processor, as /proc/cpuinfo may not report high-index offline cores.
	 */
	if (usable_processors != 1 && known_processors == 1 && last_reported_processor == last_reported_midr && cluster_count > 1) {
		cpuinfo_log_error("not sufficient per-cluster information");
	} else {
		/*
		 * Propagate MIDR, vendor, and microarchitecture values along clusters in two passes:
		 * - Copy MIDR to min processor of a cluster, if it doesn't have this information
		 * - Copy max frequency to min processor of a clsuter, if it doesn't have this information
		 * - Detect vendor and microarchitecture
		 * - Copy MIDR, vendor, and microarchitecture to all processors of a cluster, overwriting
		 *   current values for the processors in the group.
		 */

		for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
			if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
				continue;
			}

			if ((arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_MIDR) != CPUINFO_ARM_LINUX_VALID_MIDR) {
				continue;
			}

			const uint32_t group_min_processor_id = arm_linux_processors[i].package_group_min;
			if (i != group_min_processor_id) {
				if ((arm_linux_processors[group_min_processor_id].flags & CPUINFO_ARM_LINUX_VALID_MIDR) != CPUINFO_ARM_LINUX_VALID_MIDR) {
					cpuinfo_log_debug("copied MIDR %08"PRIx32" from processor %"PRIu32" to group min processor %"PRIu32,
						arm_linux_processors[i].midr, i, group_min_processor_id);
					arm_linux_processors[group_min_processor_id].midr = arm_linux_processors[i].midr;
					arm_linux_processors[group_min_processor_id].flags |= CPUINFO_ARM_LINUX_VALID_MIDR;
				}
			}
		}

		for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
			const uint32_t group_min_processor_id = arm_linux_processors[i].package_group_min;
			if (i == group_min_processor_id) {
				/* Decode vendor and uarch only once per cluster */
				cpuinfo_arm_decode_vendor_uarch(
					arm_linux_processors[i].midr,
#if CPUINFO_ARCH_ARM
					!!(arm_linux_processors[i].features & CPUINFO_ARM_LINUX_FEATURE_VFPV4),
#endif
					&arm_linux_processors[i].vendor,
					&arm_linux_processors[i].uarch);
			} else {
				arm_linux_processors[i].midr   = arm_linux_processors[group_min_processor_id].midr;
				arm_linux_processors[i].vendor = arm_linux_processors[group_min_processor_id].vendor;
				arm_linux_processors[i].uarch  = arm_linux_processors[group_min_processor_id].uarch;
			}
		}
		
	}

	/*
	 * At this point, we figured out the core clusters. Count the number of cores in each clusters:
	 * - In the first pass, for each logical processor increment the count in group-minimum processor.
	 * - In the second pass, copy the count from group-minimum processor.
	 */
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
			continue;
		}

		arm_linux_processors[arm_linux_processors[i].package_group_min].package_processor_count += 1;
	}	
	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		if ((arm_linux_processors[i].flags & CPUINFO_LINUX_MASK_USABLE) != CPUINFO_LINUX_MASK_USABLE) {
			continue;
		}

		arm_linux_processors[i].package_processor_count =
			arm_linux_processors[arm_linux_processors[i].package_group_min].package_processor_count;
	}	

	qsort(arm_linux_processors, arm_linux_processors_count,
		sizeof(struct cpuinfo_arm_linux_processor), cmp_x86_processor_by_apic_id);

	processors = calloc(usable_processors, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
			usable_processors * sizeof(struct cpuinfo_processor), usable_processors);
		goto cleanup;
	}

	for (uint32_t i = 0; i < arm_linux_processors_count; i++) {
		processors[i].vendor = arm_linux_processors[i].vendor;
		processors[i].uarch = arm_linux_processors[i].uarch;
		processors[i].topology = (struct cpuinfo_topology) {
			.thread_id = 0,
			.core_id = arm_linux_processors[i].system_processor_id,
			.package_id = 0,
			.linux_id = (int) arm_linux_processors[i].system_processor_id,
		};
	}

	/*
	 * Assumptions:
	 * - No SMP (i.e. each core supports only one hardware thread).
	 * - Level 1 instruction and data caches are private to the core clusters.
	 * - Level 2 cache is shared between cores in the same cluster.
	 */
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
			arm_linux_processors[i].architecture_version,
			&l1i[i], &l1d[i], &shared_l2);
		l1i[i].thread_start = l1d[i].thread_start = i;
		l1i[i].thread_count = l1d[i].thread_count = 1;
		#if CPUINFO_ARCH_ARM
			/* L1I reported in /proc/cpuinfo overrides defaults */
			if ((arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_ICACHE) == CPUINFO_ARM_LINUX_VALID_ICACHE) {
				l1i[i] = (struct cpuinfo_cache) {
					.size = arm_linux_processors[i].proc_cpuinfo_cache.i_size,
					.associativity = arm_linux_processors[i].proc_cpuinfo_cache.i_assoc,
					.sets = arm_linux_processors[i].proc_cpuinfo_cache.i_sets,
					.partitions = 1,
					.line_size = arm_linux_processors[i].proc_cpuinfo_cache.i_line_length
				};
			}
			/* L1D reported in /proc/cpuinfo overrides defaults */
			if ((arm_linux_processors[i].flags & CPUINFO_ARM_LINUX_VALID_ICACHE) == CPUINFO_ARM_LINUX_VALID_ICACHE) {
				l1d[i] = (struct cpuinfo_cache) {
					.size = arm_linux_processors[i].proc_cpuinfo_cache.d_size,
					.associativity = arm_linux_processors[i].proc_cpuinfo_cache.d_assoc,
					.sets = arm_linux_processors[i].proc_cpuinfo_cache.d_sets,
					.partitions = 1,
					.line_size = arm_linux_processors[i].proc_cpuinfo_cache.d_line_length
				};
			}
		#endif
		if (arm_linux_processors[i].package_group_min == arm_linux_processors[i].system_processor_id) {
			shared_l2.thread_start = i;
			shared_l2.thread_count = arm_linux_processors[i].package_processor_count;
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
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;

	cpuinfo_processors_count = usable_processors;
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
