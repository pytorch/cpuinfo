#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo.h>
#include <loongarch/linux/api.h>
#include <loongarch/api.h>
#include <loongarch/cpucfg.h>
#include <linux/api.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <cpuinfo/common.h>


#define CLUSTERS_MAX 3

static inline bool bitmask_all(uint32_t bitfield, uint32_t mask) {
	return (bitfield & mask) == mask;
}

/* Description of core clusters configuration in a chipset (identified by series) */
struct cluster_config {
	/* Number of cores (logical processors) */
	uint8_t cores;
	/* Loongarch chipset series (see cpuinfo_loongarch_chipset_series enum) */
	uint8_t series;
	/* Number of heterogenous clusters in the CPU package */
	uint8_t clusters;
	/* Number of cores in each cluster */
	uint8_t cluster_cores[CLUSTERS_MAX];
	/* CPUCFG of cores in each cluster */
	uint32_t cluster_cpucfg[CLUSTERS_MAX];
};


static const struct cluster_config cluster_configs[] = {
	{
		.cores = 4,
		.series = cpuinfo_loongarch_chipset_series_3,
	},
};

/*
 * Searches chipset name in mapping of chipset name to cores' CPUCFG values. If match is successful, initializes CPUCFG
 * for all clusters' leaders with tabulated values.
 *
 * @param[in] chipset - chipset (SoC) name information.
 * @param clusters_count - number of CPU core clusters detected in the SoC.
 * @param cluster_leaders - indices of core clusters' leaders in the @p processors array.
 * @param processors_count - number of usable logical processors in the system.
 * @param[in,out] processors - array of logical processor descriptions with pre-parsed CPUCFG, maximum frequency,
 *                             and decoded core cluster (package_leader_id) information.
 *                             Upon successful return, processors[i].cpucfg for all clusters' leaders contains the
 *                             tabulated CPUCFG values.
 * @param verify_cpucfg - indicated whether the function should check that the CPUCFG values to be assigned to leaders of
 *                      core clusters are consistent with known parts of their parsed values.
 *                      Set if to false if the only CPUCFG value parsed from /proc/cpuinfo is for the last processor
 *                      reported in /proc/cpuinfo and thus can't be unambiguously attributed to that processor.
 *
 * @retval true if the chipset was found in the mapping and core clusters' leaders initialized with CPUCFG values.
 * @retval false if the chipset was not found in the mapping, or any consistency check failed.
 */
static bool cpuinfo_loongarch_linux_detect_cluster_cpucfg_by_chipset(
	const struct cpuinfo_loongarch_chipset chipset[restrict static 1],
	uint32_t clusters_count,
	const uint32_t cluster_leaders[restrict static CLUSTERS_MAX],
	uint32_t processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static processors_count],
	bool verify_cpucfg)
{
	if (clusters_count <= CLUSTERS_MAX) {
		for (uint32_t c = 0; c < CPUINFO_COUNT_OF(cluster_configs); c++) {
			if (cluster_configs[c].series == chipset->series) {
				/* Verify that the total number of cores and clusters of cores matches expectation */
				if (cluster_configs[c].cores != processors_count || cluster_configs[c].clusters != clusters_count) {
					return false;
				}

				/* Verify that core cluster configuration matches expectation */
				for (uint32_t cluster = 0; cluster < clusters_count; cluster++) {
					const uint32_t cluster_leader = cluster_leaders[cluster];
					if (cluster_configs[c].cluster_cores[cluster] != processors[cluster_leader].package_processor_count) {
						return false;
					}
				}

				if (verify_cpucfg) {
					/* Verify known parts of CPUCFG */
					for (uint32_t cluster = 0; cluster < clusters_count; cluster++) {
						const uint32_t cluster_leader = cluster_leaders[cluster];

						/* Create a mask of known cpucfg bits */
						uint32_t cpucfg_mask = 0;
					
						if (processors[cluster_leader].flags & CPUINFO_LOONGARCH_LINUX_VALID_REVISION) {
							cpucfg_mask |= CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK;
						}

						/* Verify the bits under the mask */
						if ((processors[cluster_leader].cpucfg_id ^ cluster_configs[c].cluster_cpucfg[cluster]) & cpucfg_mask) {
							cpuinfo_log_debug("parsed CPUCFG of cluster %08"PRIu32" does not match tabulated value %08"PRIu32,
								processors[cluster_leader].cpucfg_id, cluster_configs[c].cluster_cpucfg[cluster]);
							return false;
						}
					}
				}

				/* Assign CPUCFGs according to tabulated configurations */
				for (uint32_t cluster = 0; cluster < clusters_count; cluster++) {
					const uint32_t cluster_leader = cluster_leaders[cluster];
					processors[cluster_leader].cpucfg_id = cluster_configs[c].cluster_cpucfg[cluster];
					processors[cluster_leader].flags |= CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG;
					cpuinfo_log_debug("cluster %"PRIu32" CPUCFG = 0x%08"PRIx32, cluster, cluster_configs[c].cluster_cpucfg[cluster]);
				}
				return true;
			}
		}
	}
	return false;
}


/*
 * Initializes CPUCFG for leaders of core clusters in a single sequential scan:
 *  - Clusters preceding the first reported CPUCFG value are assumed to have default CPUCFG value.
 *  - Clusters following any reported CPUCFG value to have that CPUCFG value.
 *
 * @param default_cpucfg - CPUCFG value that will be assigned to cluster leaders preceding any reported CPUCFG value.
 * @param processors_count - number of logical processor descriptions in the @p processors array.
 * @param[in,out] processors - array of logical processor descriptions with pre-parsed CPUCFG, maximum frequency,
 *                             and decoded core cluster (package_leader_id) information.
 *                             Upon successful return, processors[i].cpucfg for all core clusters' leaders contains
 *                             the assigned CPUCFG value.
 */
static void cpuinfo_loongarch_linux_detect_cluster_cpucfg_by_sequential_scan(
	uint32_t default_cpucfg,
	uint32_t processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static processors_count])
{
	uint32_t cpucfg = default_cpucfg;
	for (uint32_t i = 0; i < processors_count; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			if (processors[i].package_leader_id == i) {
				if (bitmask_all(processors[i].flags,CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG)) {
					cpucfg = processors[i].cpucfg_id;
				} else {
					cpuinfo_log_info("assume processor %"PRIu32" to have CPUCFG %08"PRIx32, i, cpucfg);
					/* To be consistent, we copy the CPUCFG entirely, rather than by parts */
					processors[i].cpucfg_id = cpucfg;
					processors[i].flags |=CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG;
				}
			}
		}
	}
}

/*
 * Detects CPUCFG of each CPU core clusters' leader.
 *
 * @param[in] chipset - chipset (SoC) name information.
 * @param max_processors - number of processor descriptions in the @p processors array.
 * @param usable_processors - number of processor descriptions in the @p processors array with both POSSIBLE and
 *                            PRESENT flags.
 * @param[in,out] processors - array of logical processor descriptions with pre-parsed CPUCFG, maximum frequency,
 *                             and decoded core cluster (package_leader_id) information.
 *                             Upon return, processors[i].cpucfg for all clusters' leaders contains the CPUCFG value.
 *
 * @returns The number of core clusters
 */
uint32_t cpuinfo_loongarch_linux_detect_cluster_cpucfg(
	const struct cpuinfo_loongarch_chipset chipset[restrict static 1],
	uint32_t max_processors,
	uint32_t usable_processors,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors])
{
	uint32_t clusters_count = 0;
	uint32_t cluster_leaders[CLUSTERS_MAX];
	uint32_t last_processor_in_cpuinfo = max_processors;
	uint32_t last_processor_with_cpucfg = max_processors;
	uint32_t processors_with_cpucfg_count = 0;
	for (uint32_t i = 0; i < max_processors; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			if (processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR) {
				last_processor_in_cpuinfo = i;
			}

			const uint32_t group_leader = processors[i].package_leader_id;
			if (group_leader == i) {
				if (clusters_count < CLUSTERS_MAX) {
					cluster_leaders[clusters_count] = i;
				}
				clusters_count += 1;
			} else {
				/* Copy known bits of information to cluster leader */
				if (!bitmask_all(processors[group_leader].flags,CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG) &&
					bitmask_all(processors[i].flags,CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG))
				{
					processors[group_leader].cpucfg_id = processors[i].cpucfg_id;
					processors[group_leader].flags |=CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG;
				}
			}
		}
	}
	cpuinfo_log_debug("detected %"PRIu32" core clusters", clusters_count);

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
	 * We detect the second case by checking if /proc/cpuinfo contains valid CPUCFG only for one,
	 * last reported, processor. Note, that the last reported core may be not the last
	 * present & possible processor, as /proc/cpuinfo may non-report high-index offline cores.
	 */
	
	if (processors_with_cpucfg_count < usable_processors) {
		/*
		 * /proc/cpuinfo reported CPUCFG only for some processors, and probably some core clusters do not have CPUCFG
		 * for any of the cores. Check if this is the case.
		 */
		uint32_t clusters_with_cpucfg_count = 0;
		for (uint32_t i = 0; i < max_processors; i++) {
			if (bitmask_all(processors[i].flags, CPUINFO_LINUX_FLAG_VALID |CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG)) {
				if (processors[i].package_leader_id == i) {
					clusters_with_cpucfg_count += 1;
				}
			}
		}
		
		if (clusters_with_cpucfg_count < clusters_count) {
			/*
			 * /proc/cpuinfo reported CPUCFG only for some clusters, need to reconstruct others.
			 * We make three attempts to detect CPUCFG for clusters without it:
			 * 1. Search tabulated CPUCFG values for chipsets which have heterogeneous clusters and ship with Linux
			 *    kernels which do not always report all cores in /proc/cpuinfo. If found, use the tabulated values.
			 * 2. For systems with 2 clusters and CPUCFG known for one cluster, assume big.LITTLE configuration,
			 *    and estimate CPUCFG for the other cluster under assumption that CPUCFG for the big cluster is known.
			 * 3. Initialize CPUCFGs for core clusters in a single sequential scan:
			 *    - Clusters preceding the first reported CPUCFG value are assumed to have the last reported CPUCFG value.
			 *    - Clusters following any reported CPUCFG value to have that CPUCFG value.
			 */

			if (cpuinfo_loongarch_linux_detect_cluster_cpucfg_by_chipset(
				chipset, clusters_count, cluster_leaders, usable_processors, processors, true))
			{
				return clusters_count;
			}

			if (last_processor_with_cpucfg != max_processors) {
				/* Fall back to sequential initialization of CPUCFG values for core clusters */
				cpuinfo_loongarch_linux_detect_cluster_cpucfg_by_sequential_scan(
					processors[processors[last_processor_with_cpucfg].package_leader_id].cpucfg_id,
					max_processors, processors);
			}
		}
	}
	return clusters_count;
}
