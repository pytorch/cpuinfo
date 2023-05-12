#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <linux/api.h>
#include <riscv/api.h>
#include <riscv/linux/api.h>

/* ISA structure to hold supported extensions. */
struct cpuinfo_riscv_isa cpuinfo_isa;

static struct cpuinfo_package package;

/* Helper function to bitmask flags and ensure operator precedence. */
static inline bool bitmask_all(uint32_t flags, uint32_t mask) {
  return (flags & mask) == mask;
}

static inline uint32_t min(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}

static inline int cmp(uint32_t a, uint32_t b) {
    return (a > b) - (a < b);
}

static int compare_riscv_linux_processors(const void* a, const void* b) {
    const struct cpuinfo_riscv_linux_processor* processor_a = (const struct cpuinfo_riscv_linux_processor*) a;
    const struct cpuinfo_riscv_linux_processor* processor_b = (const struct cpuinfo_riscv_linux_processor*) b;

    /* Move usable processors towards the start of the array */
    const bool usable_a = bitmask_all(processor_a->flags, CPUINFO_LINUX_FLAG_VALID);
    const bool usable_b = bitmask_all(processor_b->flags, CPUINFO_LINUX_FLAG_VALID);
    if (usable_a != usable_b) {
        return (int) usable_b - (int) usable_a;
    }

    /* Compare based on micro-architecture (i.e. uarch 0 < uarch 1) */
    const uint32_t uarch_a = processor_a->core.uarch;
    const uint32_t uarch_b = processor_b->core.uarch;
    if (uarch_a != uarch_b) {
        return uarch_b - uarch_a;
    }

    /* Compare based on system processor id (i.e. processor 0 < processor 1) */
    const uint32_t id_a = processor_a->processor.linux_id;
    const uint32_t id_b = processor_b->processor.linux_id;
    return cmp(id_a, id_b);
}

/**
 * Parses the core cpus list for each processor. This function is called once
 * per-processor, with the IDs of all other processors in the core list.
 *
 * The 'processor_[start|count]' are populated in the processor's 'core'
 * attribute, with 'start' being the smallest ID in the core list.
 *
 * The 'core_leader_id' of each processor is set to the smallest ID in it's
 * cluster CPU list.
 *
 * Precondition: The element in the 'processors' list must be initialized with
 * their 'core_leader_id' to their index in the list.

 * E.g. processors[0].core_leader_id = 0.
 */
static bool core_cpus_parser(uint32_t processor,
			     uint32_t core_cpus_start,
			     uint32_t core_cpus_end,
			     struct cpuinfo_riscv_linux_processor* processors) {
	uint32_t processor_start = UINT32_MAX;
	uint32_t processor_count = 0;

	/* If the processor already has a leader, use it. */
	if (bitmask_all(processors[processor].flags, CPUINFO_LINUX_FLAG_CORE_CLUSTER)) {
		processor_start = processors[processor].core_leader_id;
	}

	for (size_t core_cpu = core_cpus_start; core_cpu < core_cpus_end; core_cpu++) {
		if (!bitmask_all(processors[core_cpu].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}
		/**
		 * The first valid processor observed is the smallest ID in the
		 * list that attaches to this core.
		 */
		if (processor_start == UINT32_MAX) {
			processor_start = core_cpu;
		}
		processors[core_cpu].core_leader_id = processor_start;
		processor_count++;
	}
	/**
	 * If the cluster flag has not been set, assign the processor start. If
	 * it has been set, only apply the processor start if it's less than the
	 * held value. This can happen if the callback is invoked twice:
	 *
	 * e.g. core_cpu_list=1,10-12
	 */
	if (!bitmask_all(processors[processor].flags, CPUINFO_LINUX_FLAG_CORE_CLUSTER)
	    || processors[processor].core.processor_start > processor_start) {
		processors[processor].core.processor_start = processor_start;
		processors[processor].core_leader_id = processor_start;
	}
	processors[processor].core.processor_count += processor_count;
	processors[processor].flags |= CPUINFO_LINUX_FLAG_CORE_CLUSTER;
	/* The parser has failed only if no processors were found. */
	return processor_count != 0;
}

/**
 * Parses the cluster cpu list for each processor. This function is called once
 * per-processor, with the IDs of all other processors in the cluster.
 *
 * The 'cluster_leader_id' of each processor is set to the smallest ID in it's
 * cluster CPU list.
 *
 * Precondition: The element in the 'processors' list must be initialized with
 * their 'cluster_leader_id' to their index in the list.
 * E.g. processors[0].cluster_leader_id = 0.
 */
static bool cluster_cpus_parser(uint32_t processor,
				uint32_t cluster_cpus_start,
				uint32_t cluster_cpus_end,
				struct cpuinfo_riscv_linux_processor* processors) {
	uint32_t processor_start = UINT32_MAX;
	uint32_t processor_count = 0;
	uint32_t core_count = 0;

	/* If the processor already has a leader, use it. */
	if (bitmask_all(processors[processor].flags, CPUINFO_LINUX_FLAG_CLUSTER_CLUSTER)) {
		processor_start = processors[processor].cluster_leader_id;
	}

	for (size_t cluster_cpu = cluster_cpus_start; cluster_cpu < cluster_cpus_end; cluster_cpu++) {
		if (!bitmask_all(processors[cluster_cpu].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}
		/**
		 * The first valid processor observed is the smallest ID in the
		 * list that attaches to this core.
		 */
		if (processor_start == UINT32_MAX) {
			processor_start = cluster_cpu;
		}
		processors[cluster_cpu].cluster_leader_id = processor_start;
		processor_count++;
		/**
		 * A processor should only represent it's core if it is the
		 * assigned leader of that core.
		 */
		if (processors[cluster_cpu].core_leader_id == cluster_cpu) {
			core_count++;
		}
	}
	/**
	 * If the cluster flag has not been set, assign the processor start. If
	 * it has been set, only apply the processor start if it's less than the
	 * held value. This can happen if the callback is invoked twice:
	 *
	 * e.g. cluster_cpus_list=1,10-12
	 */
	if (!bitmask_all(processors[processor].flags, CPUINFO_LINUX_FLAG_CLUSTER_CLUSTER)
	    || processors[processor].cluster.processor_start > processor_start) {
		processors[processor].cluster.processor_start = processor_start;
		processors[processor].cluster.core_start = processor_start;
		processors[processor].cluster.cluster_id = processor_start;
		processors[processor].cluster_leader_id = processor_start;
	}
	processors[processor].cluster.processor_count += processor_count;
	processors[processor].cluster.core_count += core_count;
	processors[processor].flags |= CPUINFO_LINUX_FLAG_CLUSTER_CLUSTER;
	return true;
}

/**
 * Parses the package cpus list for each processor. This function is called once
 * per-processor, with the IDs of all other processors in the package list.
 *
 * The 'processor_[start|count]' are populated in the processor's 'package'
 * attribute, with 'start' being the smallest ID in the package list.
 *
 * The 'package_leader_id' of each processor is set to the smallest ID in it's
 * cluster CPU list.
 *
 * Precondition: The element in the 'processors' list must be initialized with
 * their 'package_leader_id' to their index in the list.
 * E.g. processors[0].package_leader_id = 0.
 */
static bool package_cpus_parser(uint32_t processor,
			     uint32_t package_cpus_start,
			     uint32_t package_cpus_end,
			     struct cpuinfo_riscv_linux_processor* processors) {
	uint32_t processor_start = UINT32_MAX;
	uint32_t processor_count = 0;
	uint32_t cluster_count = 0;
	uint32_t core_count = 0;

	/* If the processor already has a leader, use it. */
	if (bitmask_all(processors[processor].flags, CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER)) {
		processor_start = processors[processor].package_leader_id;
	}

	for (size_t package_cpu = package_cpus_start; package_cpu < package_cpus_end; package_cpu++) {
		if (!bitmask_all(processors[package_cpu].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}
		/**
		 * The first valid processor observed is the smallest ID in the
		 * list that attaches to this package.
		 */
		if (processor_start == UINT32_MAX) {
			processor_start = package_cpu;
		}
		processors[package_cpu].package_leader_id = processor_start;
		processor_count++;
		/**
		 * A processor should only represent it's core if it is the
		 * assigned leader of that core, and similarly for it's cluster.
		 */
		if (processors[package_cpu].cluster_leader_id == package_cpu) {
			cluster_count++;
		}
		if (processors[package_cpu].core_leader_id == package_cpu) {
			core_count++;
		}
	}
	/**
	 * If the cluster flag has not been set, assign the processor start. If
	 * it has been set, only apply the processor start if it's less than the
	 * held value. This can happen if the callback is invoked twice:
	 *
	 * e.g. package_cpus_list=1,10-12
	 */
	if (!bitmask_all(processors[processor].flags, CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER)
	    || processors[processor].package.processor_start > processor_start) {
		processors[processor].package.processor_start = processor_start;
		processors[processor].package.cluster_start = processor_start;
		processors[processor].package.core_start = processor_start;
		processors[processor].package_leader_id = processor_start;
	}
	processors[processor].package.processor_count += processor_count;
	processors[processor].package.cluster_count += cluster_count;
	processors[processor].package.core_count += core_count;
	processors[processor].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
	return true;
}

/* Initialization for the RISC-V Linux system. */
void cpuinfo_riscv_linux_init(void) {
	struct cpuinfo_riscv_linux_processor* riscv_linux_processors = NULL;
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_package* packages = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_uarch_info* uarchs = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	const struct cpuinfo_processor** linux_cpu_to_processor_map = NULL;
	const struct cpuinfo_core** linux_cpu_to_core_map = NULL;
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
	uint32_t riscv_linux_processors_count = max_processors_count;
	if (max_present_processors_count != 0) {
		riscv_linux_processors_count = min(riscv_linux_processors_count, max_present_processors_count);
		valid_processor_mask |= CPUINFO_LINUX_FLAG_PRESENT;
	}
	if (max_possible_processors_count != 0) {
		riscv_linux_processors_count = min(riscv_linux_processors_count, max_possible_processors_count);
		valid_processor_mask |= CPUINFO_LINUX_FLAG_POSSIBLE;
	}
	if ((max_present_processors_count | max_possible_processors_count) == 0) {
		cpuinfo_log_error("failed to parse both lists of possible and present processors");
		return;
	}

	riscv_linux_processors = calloc(riscv_linux_processors_count, sizeof(struct cpuinfo_riscv_linux_processor));
	if (riscv_linux_processors == NULL) {
		cpuinfo_log_error(
				"failed to allocate %zu bytes for descriptions of %"PRIu32" RISC-V logical processors",
				riscv_linux_processors_count * sizeof(struct cpuinfo_riscv_linux_processor),
				riscv_linux_processors_count);
		goto cleanup;
	}

	/**
	 * Attempt to detect all processors and apply the corresponding flag to
	 * each processor struct that we find.
	 */
	if (max_possible_processors_count) {
		if (!cpuinfo_linux_detect_possible_processors(
				riscv_linux_processors_count, &riscv_linux_processors->flags,
				sizeof(struct cpuinfo_riscv_linux_processor),
				valid_processor_mask)) {
			cpuinfo_log_error("failed to detect possible processors");
			goto cleanup;
		}
	}

	if (max_present_processors_count) {
		if (!cpuinfo_linux_detect_present_processors(
				riscv_linux_processors_count, &riscv_linux_processors->flags,
				sizeof(struct cpuinfo_riscv_linux_processor),
				valid_processor_mask)) {
			cpuinfo_log_error("failed to detect present processors");
			goto cleanup;
		}
	}

	for (size_t processor = 0; processor < riscv_linux_processors_count; processor++) {
		if (bitmask_all(riscv_linux_processors[processor].flags, valid_processor_mask)) {
			riscv_linux_processors[processor].processor.linux_id = processor;
			riscv_linux_processors[processor].flags |= CPUINFO_LINUX_FLAG_VALID;
			cpuinfo_log_debug("parsed processor %"PRIu32, processor);
		}
	}

	for (size_t processor = 0; processor < riscv_linux_processors_count; processor++) {
		if (!bitmask_all(riscv_linux_processors[processor].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}

		/* Populate processor start and count information. */
		if (!cpuinfo_linux_detect_core_cpus(
				riscv_linux_processors_count,
				processor,
				(cpuinfo_siblings_callback) core_cpus_parser,
				riscv_linux_processors)) {
			cpuinfo_log_error("failed to detect core cpus for processor %zu.", processor);
			goto cleanup;
		}

		/* Populate core ID information. */
		if (cpuinfo_linux_get_processor_core_id(
				processor,
				&riscv_linux_processors[processor].core.core_id)) {
			riscv_linux_processors[processor].flags |= CPUINFO_LINUX_FLAG_CORE_ID;
		}

		/**
		 * Populate the vendor and uarch of this core from this
		 * processor. When the final 'cores' list is constructed, only
		 * the values from the core leader will be honored.
		 */
		cpuinfo_riscv_linux_decode_vendor_uarch_from_hwprobe(
				processor,
				&riscv_linux_processors[processor].core.vendor,
				&riscv_linux_processors[processor].core.uarch);

		/* Populate frequency information of this core. */
		uint32_t frequency = cpuinfo_linux_get_processor_cur_frequency(processor);
		if (frequency != 0) {
			riscv_linux_processors[processor].core.frequency = frequency;
			riscv_linux_processors[processor].flags |= CPUINFO_LINUX_FLAG_CUR_FREQUENCY;
		}
	}

	/* Populate cluster information. */
	for (size_t processor = 0; processor < riscv_linux_processors_count; processor++) {
		if (!bitmask_all(riscv_linux_processors[processor].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}
		if (!cpuinfo_linux_detect_cluster_cpus(
				riscv_linux_processors_count,
				processor,
				(cpuinfo_siblings_callback) cluster_cpus_parser,
				riscv_linux_processors)) {
			cpuinfo_log_warning("failed to detect cluster cpus for processor %zu.", processor);
			goto cleanup;
		}

		/**
		 * Populate the vendor, uarch and frequency of this cluster from
		 * this logical processor. When the 'clusters' list is constructed,
		 * only the values from the cluster leader will be honored.
		 */
		riscv_linux_processors[processor].cluster.vendor =
				riscv_linux_processors[processor].core.vendor;
		riscv_linux_processors[processor].cluster.uarch =
				riscv_linux_processors[processor].core.uarch;
		riscv_linux_processors[processor].cluster.frequency =
				riscv_linux_processors[processor].core.frequency;
	}

	/* Populate package information. */
	for (size_t processor = 0; processor < riscv_linux_processors_count; processor++) {
		if (!bitmask_all(riscv_linux_processors[processor].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}
		if (!cpuinfo_linux_detect_package_cpus(
				riscv_linux_processors_count,
				processor,
				(cpuinfo_siblings_callback) package_cpus_parser,
				riscv_linux_processors)) {
			cpuinfo_log_warning("failed to detect package cpus for processor %zu.", processor);
			goto cleanup;
		}
	}

	/* Populate ISA structure with hwcap information. */
	cpuinfo_riscv_linux_decode_isa_from_hwcap(&cpuinfo_isa);

	/**
	 * To efficiently compute the number of unique micro-architectures
	 * present on the system, sort the processor list by micro-architecture
	 * and then scan through the list to count the differences.
	 *
	 * Ensure this is done at the end of composing the processor list - the
	 * parsing functions assume that the position of the processor in the
	 * list matches it's Linux ID, which this sorting operation breaks.
	 */
	qsort(riscv_linux_processors,
		  riscv_linux_processors_count,
	      sizeof(struct cpuinfo_riscv_linux_processor),
	      compare_riscv_linux_processors);

	/**
	 * Determine the number of *valid* detected processors, cores,
	 * clusters, packages and uarchs in the list.
	 */
	size_t valid_processors_count = 0;
	size_t valid_cores_count = 0;
	size_t valid_clusters_count = 0;
	size_t valid_packages_count = 0;
	size_t valid_uarchs_count = 0;
	enum cpuinfo_uarch last_uarch = cpuinfo_uarch_unknown;
	for (size_t processor = 0; processor < riscv_linux_processors_count; processor++) {
		if (!bitmask_all(riscv_linux_processors[processor].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}

		/**
		 * All comparisons to the leader id values MUST be done against
		 * the 'linux_id' as opposed to 'processor'. The sort function
		 * above no longer allows us to make the assumption that these
		 * two values are the same.
		 */
		uint32_t linux_id = riscv_linux_processors[processor].processor.linux_id;

		valid_processors_count++;
		if (riscv_linux_processors[processor].core_leader_id == linux_id) {
			valid_cores_count++;
		}
		if (riscv_linux_processors[processor].cluster_leader_id == linux_id) {
			valid_clusters_count++;
		}
		if (riscv_linux_processors[processor].package_leader_id == linux_id) {
			valid_packages_count++;
		}
		/**
		 * As we've sorted by micro-architecture, when the uarch differs
		 * between two entries, a unique uarch has been observed.
		 */
		if (last_uarch != riscv_linux_processors[processor].core.uarch
			|| valid_uarchs_count == 0) {
			valid_uarchs_count++;
			last_uarch = riscv_linux_processors[processor].core.uarch;
		}
	}

	/* Allocate and populate final public ABI structures. */
	processors = calloc(valid_processors_count,
			    sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %zu processors.",
			valid_processors_count * sizeof(struct cpuinfo_processor),
			valid_processors_count);
		goto cleanup;
	}

	cores = calloc(valid_cores_count,
			    sizeof(struct cpuinfo_core));
	if (cores == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %zu cores.",
			valid_cores_count * sizeof(struct cpuinfo_core),
			valid_cores_count);
		goto cleanup;
	}

	clusters = calloc(valid_clusters_count,
			    sizeof(struct cpuinfo_cluster));
	if (clusters == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %zu clusters.",
			valid_clusters_count * sizeof(struct cpuinfo_cluster),
			valid_clusters_count);
		goto cleanup;
	}

	packages = calloc(valid_packages_count,
			    sizeof(struct cpuinfo_package));
	if (packages == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %zu packages.",
			valid_packages_count * sizeof(struct cpuinfo_package),
			valid_packages_count);
		goto cleanup;
	}

	uarchs = calloc(valid_uarchs_count, sizeof(struct cpuinfo_uarch_info));
	if (uarchs == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %zu packages.",
			valid_uarchs_count * sizeof(struct cpuinfo_uarch_info),
			valid_uarchs_count);
		goto cleanup;
	}

	linux_cpu_to_processor_map = calloc(riscv_linux_processors_count,
					    sizeof(struct cpuinfo_processor*));
	if (linux_cpu_to_processor_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" processor map.",
						  riscv_linux_processors_count * sizeof(struct cpuinfo_processor*),
						  riscv_linux_processors_count);
		goto cleanup;
	}

	linux_cpu_to_core_map = calloc(riscv_linux_processors_count,
				       sizeof(struct cpuinfo_core*));
	if (linux_cpu_to_core_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" core map.",
						  riscv_linux_processors_count * sizeof(struct cpuinfo_core*),
						  riscv_linux_processors_count);
		goto cleanup;
	}

	linux_cpu_to_uarch_index_map = calloc(riscv_linux_processors_count,
					      sizeof(struct cpuinfo_uarch_info*));
	if (linux_cpu_to_uarch_index_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" uarch map.",
						  riscv_linux_processors_count * sizeof(struct cpuinfo_uarch_info*),
						  riscv_linux_processors_count);
		goto cleanup;
	}

	/* Transfer contents of processor list to ABI structures. */
	size_t valid_processors_index = 0;
	size_t valid_cores_index = 0;
	size_t valid_clusters_index = 0;
	size_t valid_packages_index = 0;
	size_t valid_uarchs_index = 0;
	last_uarch = cpuinfo_uarch_unknown;
	for (size_t processor = 0; processor < riscv_linux_processors_count; processor++) {
		if (!bitmask_all(riscv_linux_processors[processor].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}

		/**
		 * All comparisons to the leader id values MUST be done against
		 * the 'linux_id' as opposed to 'processor'. The sort function
		 * above no longer allows us to make the assumption that these
		 * two values are the same.
		 */
		uint32_t linux_id = riscv_linux_processors[processor].processor.linux_id;

		/* Create uarch entry if this uarch has not been seen before. */
		if (last_uarch != riscv_linux_processors[processor].core.uarch
		    || valid_uarchs_index == 0) {
			uarchs[valid_uarchs_index++].uarch =
					riscv_linux_processors[processor].core.uarch;
			last_uarch = riscv_linux_processors[processor].core.uarch;
		}

		/* Copy cpuinfo_processor information. */
		memcpy(&processors[valid_processors_index++],
		       &riscv_linux_processors[processor].processor,
			sizeof(struct cpuinfo_processor));

		/* Update uarch processor count. */
		uarchs[valid_uarchs_index - 1].processor_count++;

		/* Copy cpuinfo_core information, if this is the leader. */
		if (riscv_linux_processors[processor].core_leader_id == linux_id) {
			memcpy(&cores[valid_cores_index++],
			       &riscv_linux_processors[processor].core,
			       sizeof(struct cpuinfo_core));
			/* Update uarch core count. */
			uarchs[valid_uarchs_index - 1].core_count++;
		}

		/* Copy cpuinfo_cluster information, if this is the leader. */
		if (riscv_linux_processors[processor].cluster_leader_id == linux_id) {
			memcpy(&clusters[valid_clusters_index++],
			       &riscv_linux_processors[processor].cluster,
			       sizeof(struct cpuinfo_cluster));
		}

		/* Copy cpuinfo_package information, if this is the leader. */
		if (riscv_linux_processors[processor].package_leader_id == linux_id) {
			memcpy(&packages[valid_packages_index++],
			       &riscv_linux_processors[processor].package,
			       sizeof(struct cpuinfo_package));
		}

		/* Commit pointers on the final structures. */
		processors[valid_processors_index - 1].core = &cores[valid_cores_index - 1];
		processors[valid_processors_index - 1].cluster = &clusters[valid_clusters_index - 1];
		processors[valid_processors_index - 1].package = &packages[valid_packages_index - 1];

		cores[valid_cores_index - 1].cluster = &clusters[valid_clusters_index - 1];
		cores[valid_cores_index - 1].package = &packages[valid_packages_index - 1];

		clusters[valid_clusters_index - 1].package = &packages[valid_packages_index - 1];

		linux_cpu_to_processor_map[linux_id] = &processors[valid_processors_index - 1];
		linux_cpu_to_core_map[linux_id] = &cores[valid_cores_index - 1];
		linux_cpu_to_uarch_index_map[linux_id] = valid_uarchs_index - 1;
	}

    uint32_t l2_index = UINT32_MAX;
    for (uint32_t processor = 0; processor < valid_processors_count; processor++) {

        struct cpuinfo_cache dummy_l1i, dummy_l1d, temp_l2 = { 0 };
        cpuinfo_riscv_decode_cache(
                riscv_linux_processors[processor].core.uarch,
                &dummy_l1i, &dummy_l1d, &temp_l2);

        if (temp_l2.size != 0) {
            if (riscv_linux_processors[processor].package_leader_id == riscv_linux_processors[processor].processor.linux_id) {
                l2_index += 1;
                l2[l2_index] = (struct cpuinfo_cache) {
                        .size            = temp_l2.size,
                        .associativity   = temp_l2.associativity,
                        .sets            = temp_l2.sets,
                        .partitions      = temp_l2.partitions,
                        .line_size       = temp_l2.line_size,
                        .flags           = temp_l2.flags,
                        .processor_start = processor,
                        .processor_count = riscv_linux_processors[processor].package.processor_count,
                };
            }
            processors[processor].cache.l2 = l2 + l2_index;
        }
    }

	/* Commit */
	cpuinfo_processors = processors;
	cpuinfo_processors_count = valid_processors_count;
	cpuinfo_cores = cores;
	cpuinfo_cores_count = valid_cores_count;
	cpuinfo_clusters = clusters;
	cpuinfo_clusters_count = valid_clusters_count;
	cpuinfo_packages = packages;
	cpuinfo_packages_count = valid_packages_count;
	cpuinfo_uarchs = uarchs;
	cpuinfo_uarchs_count = valid_uarchs_count;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = valid_processors_count;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = valid_processors_count;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = valid_clusters_count;
	cpuinfo_max_cache_size = cpuinfo_riscv_compute_max_cache_size(&processors[0]);


	cpuinfo_linux_cpu_max = riscv_linux_processors_count;
	cpuinfo_linux_cpu_to_processor_map = linux_cpu_to_processor_map;
	cpuinfo_linux_cpu_to_core_map = linux_cpu_to_core_map;
	cpuinfo_linux_cpu_to_uarch_index_map = linux_cpu_to_uarch_index_map;

	__sync_synchronize();

	cpuinfo_is_initialized = true;

	/* Mark all public structures NULL to prevent cleanup from erasing them. */
	processors = NULL;
	cores = NULL;
	clusters = NULL;
	packages = NULL;
	uarchs = NULL;
	l1i = l1d = l2 = NULL;
	linux_cpu_to_processor_map = NULL;
	linux_cpu_to_core_map = NULL;
	linux_cpu_to_uarch_index_map = NULL;
cleanup:
	free(riscv_linux_processors);
	free(processors);
	free(cores);
	free(clusters);
	free(packages);
	free(uarchs);
	free(l1i);
	free(l1d);
	free(l2);
	free(linux_cpu_to_processor_map);
	free(linux_cpu_to_core_map);
	free(linux_cpu_to_uarch_index_map);
}
