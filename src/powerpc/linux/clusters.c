#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <linux/api.h>
#include <powerpc/pvr.h>
#include <powerpc/linux/api.h>
#include <cpuinfo/log.h>

#define CLUSTERS_MAX 4

static inline bool bitmask_all(uint32_t bitfield, uint32_t mask) {
	return (bitfield & mask) == mask;
}

static void cpuinfo_powerpc_linux_detect_cluster_pvr_by_sequential_scan(
	uint32_t default_pvr,
	uint32_t processors_count,
	struct cpuinfo_powerpc_linux_processor processors[restrict static processors_count])
{
	uint32_t pvr = default_pvr;
	for (uint32_t i = 0; i < processors_count; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			if (processors[i].package_leader_id == i) {
				if (bitmask_all(processors[i].flags, CPUINFO_POWERPC_LINUX_VALID_PVR)) {
					pvr = processors[i].pvr;
				} else {
					cpuinfo_log_info("assume processor %"PRIu32" to have PVR %08"PRIx32, i, pvr);
					processors[i].pvr = pvr;
					processors[i].flags |= CPUINFO_POWERPC_LINUX_VALID_PVR;
				}
			}
		}
	}
}

uint32_t cpuinfo_powerpc_linux_detect_cluster_pvr(
	uint32_t max_processors,
	uint32_t usable_processors,
	struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors])
{
	uint32_t clusters_count = 0;
	uint32_t cluster_leaders[CLUSTERS_MAX];
	uint32_t last_processor_in_cpuinfo = max_processors;
	uint32_t last_processor_with_pvr = max_processors;
	uint32_t processors_with_pvr_count = 0;
	for (uint32_t i = 0; i < max_processors; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			if (processors[i].flags & CPUINFO_POWERPC_LINUX_VALID_PROCESSOR) {
				last_processor_in_cpuinfo = i;
			}
			if (bitmask_all(processors[i].flags, CPUINFO_POWERPC_LINUX_VALID_REVISION | CPUINFO_POWERPC_LINUX_VALID_VERSION)) {
				last_processor_with_pvr = i;
				processors_with_pvr_count += 1;
			}
			const uint32_t group_leader = processors[i].package_leader_id;
			if (group_leader == i) {
				if (clusters_count < CLUSTERS_MAX) {
					cluster_leaders[clusters_count] = i;
				}
				clusters_count += 1;
			} else {
				/* Copy known bits of information to cluster leader */
				if ((processors[i].flags & ~processors[group_leader].flags) & CPUINFO_LINUX_FLAG_MAX_FREQUENCY) {
					processors[group_leader].max_frequency = processors[i].max_frequency;
					processors[group_leader].flags |= CPUINFO_LINUX_FLAG_MAX_FREQUENCY;
				}
				if (!bitmask_all(processors[group_leader].flags, CPUINFO_POWERPC_LINUX_VALID_PVR) &&
					bitmask_all(processors[i].flags, CPUINFO_POWERPC_LINUX_VALID_PVR))
				{
					processors[group_leader].pvr = processors[i].pvr;
					processors[group_leader].flags |= CPUINFO_POWERPC_LINUX_VALID_PVR;
				}
			}
		}
	}
	cpuinfo_log_debug("detected %"PRIu32" core clusters", clusters_count);

	if (processors_with_pvr_count == 1 && last_processor_in_cpuinfo == last_processor_with_pvr && clusters_count > 1) {

		cpuinfo_powerpc_linux_detect_cluster_pvr_by_sequential_scan(
			processors[processors[last_processor_with_pvr].package_leader_id].pvr,
			max_processors, processors);
	} else if (processors_with_pvr_count < usable_processors) {

		uint32_t clusters_with_pvr_count = 0;
		for (uint32_t i = 0; i < max_processors; i++) {
			if (bitmask_all(processors[i].flags, CPUINFO_LINUX_MASK_USABLE | CPUINFO_POWERPC_LINUX_VALID_PVR)) {
				if (processors[i].package_leader_id == i) {
					clusters_with_pvr_count += 1;
				}
			}
		}

		if (clusters_with_pvr_count < clusters_count) {

				cpuinfo_powerpc_linux_detect_cluster_pvr_by_sequential_scan(
					processors[processors[last_processor_with_pvr].package_leader_id].pvr,
					max_processors, processors);
		}
	}
	return clusters_count;
}

/*
 * Counts the number of logical processors in each core cluster.
 *
*/
void cpuinfo_powerpc_linux_count_cluster_processors(
	uint32_t max_processors,
	struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors])
{
	/* First pass: accumulate the number of processors at the group leader's package_processor_count */
	for (uint32_t i = 0; i< max_processors; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			const uint32_t package_leader_id = processors[i].package_leader_id;
			processors[package_leader_id].package_processor_count += 1;
		}
	}

	/* Second pass: copy the package_processor_count from the group leader processor */
	for (uint32_t i = 0; i< max_processors; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_MASK_USABLE)) {
			const uint32_t package_leader_id = processors[i].package_leader_id;
			processors[i].package_processor_count = processors[package_leader_id].package_processor_count;
		}
	}
}

void cpuinfo_powerpc_linux_detect_core_clusters_by_sequential_scan(
	uint32_t max_processors,
	struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors])
{
	uint32_t cluster_flags = 0;
	uint32_t cluster_processors = 0;
	uint32_t cluster_start;
	uint32_t cluster_pvr;
	uint32_t cluster_max_frequency, cluster_min_frequency;

	for (uint32_t i=0; i < max_processors; i++) {
		if ((processors[i].flags & (CPUINFO_LINUX_MASK_USABLE | CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER)) == CPUINFO_LINUX_MASK_USABLE) {
			if (cluster_processors == 0) {
				goto new_cluster;
			}

			if (processors[i].flags & CPUINFO_LINUX_FLAG_MIN_FREQUENCY) {
				if (cluster_flags & CPUINFO_LINUX_FLAG_MIN_FREQUENCY) {
					if (cluster_min_frequency != processors[i].min_frequency) {
						cpuinfo_log_info(
							"minimum frequency of processor %"PRIu32" (%"PRIu32" KHz) is different than of preceeding cluster (%"PRIu32" KHz); "
							"processor %"PRIu32" starts to a new cluster",
							i, processors[i].min_frequency, cluster_min_frequency, i);
						goto new_cluster;
					}
				} else {
					cluster_min_frequency = processors[i].min_frequency;
					cluster_flags |= CPUINFO_LINUX_FLAG_MIN_FREQUENCY;
				}
			}

			if (processors[i].flags & CPUINFO_LINUX_FLAG_MAX_FREQUENCY) {
				if (cluster_flags & CPUINFO_LINUX_FLAG_MAX_FREQUENCY) {
					if (cluster_max_frequency != processors[i].max_frequency) {
						cpuinfo_log_debug(
							"maximum frequency of processor %"PRIu32" (%"PRIu32" KHz) is different than of preceeding cluster (%"PRIu32" KHz); "
							"processor %"PRIu32" starts a new cluster",
							i, processors[i].max_frequency, cluster_max_frequency, i);
						goto new_cluster;
					}
				} else {
					cluster_max_frequency = processors[i].max_frequency;
					cluster_flags |= CPUINFO_LINUX_FLAG_MAX_FREQUENCY;
				}
			}

			if (processors[i].flags & CPUINFO_POWERPC_LINUX_VALID_VERSION) {
				if (cluster_flags & CPUINFO_POWERPC_LINUX_VALID_VERSION) {
					if ((cluster_pvr & CPUINFO_POWERPC_PVR_VERSION_MASK) != (processors[i].pvr & CPUINFO_POWERPC_PVR_VERSION_MASK)) {
						cpuinfo_log_debug(
							"CPU Version of processor %"PRIu32" (0x%02"PRIx32") is different than of preceeding cluster (0x%02"PRIx32"); "
							"processor %"PRIu32" starts to a new cluster",
							i, pvr_get_version(processors[i].pvr), pvr_get_version(cluster_pvr), i);
						goto new_cluster;
					}
				} else {
					cluster_pvr = pvr_copy_version(cluster_pvr, processors[i].pvr);
					cluster_flags |= CPUINFO_POWERPC_LINUX_VALID_VERSION;
				}
			}

			if (processors[i].flags & CPUINFO_POWERPC_LINUX_VALID_REVISION) {
				if (cluster_flags & CPUINFO_POWERPC_LINUX_VALID_REVISION) {
					if ((cluster_pvr & CPUINFO_POWERPC_PVR_REVISION_MASK) != (processors[i].pvr & CPUINFO_POWERPC_PVR_REVISION_MASK)) {
						cpuinfo_log_debug(
							"CPU Revision of processor %"PRIu32" (0x%02"PRIx32") is different than of preceeding cluster (0x%02"PRIx32"); "
							"processor %"PRIu32" starts to a new cluster",
							i, pvr_get_version(processors[i].pvr), pvr_get_version(cluster_pvr), i);
						goto new_cluster;
					}
				} else {
					cluster_pvr = pvr_copy_revision(cluster_pvr, processors[i].pvr);
					cluster_flags |= CPUINFO_POWERPC_LINUX_VALID_REVISION;
				}
			}

			/* All checks passed, attach processor to the preceeding cluster */
			cluster_processors++;
			processors[i].package_leader_id = cluster_start;
			processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
			cpuinfo_log_debug("assigned processor %"PRIu32" to preceeding cluster of processor %"PRIu32, i, cluster_start);
			continue;

new_cluster:
			/* Create a new cluster starting with processor i */
			cluster_start = i;
			processors[i].package_leader_id = i;
			processors[i].flags |= CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER;
			cluster_processors = 1;

			/* Copy known information from processor to cluster, and set the flags accordingly */
			cluster_flags = 0;
			if (processors[i].flags & CPUINFO_LINUX_FLAG_MIN_FREQUENCY) {
				cluster_min_frequency = processors[i].min_frequency;
				cluster_flags |= CPUINFO_LINUX_FLAG_MIN_FREQUENCY;
			}
			if (processors[i].flags & CPUINFO_LINUX_FLAG_MAX_FREQUENCY) {
				cluster_max_frequency = processors[i].max_frequency;
				cluster_flags |= CPUINFO_LINUX_FLAG_MAX_FREQUENCY;
			}
			if (processors[i].flags & CPUINFO_POWERPC_LINUX_VALID_VERSION) {
				cluster_pvr = pvr_copy_version(cluster_pvr, processors[i].pvr);
				cluster_flags |= CPUINFO_POWERPC_LINUX_VALID_VERSION;
			}
			if (processors[i].flags & CPUINFO_POWERPC_LINUX_VALID_REVISION) {
				cluster_pvr = pvr_copy_revision(cluster_pvr, processors[i].pvr);
				cluster_flags |= CPUINFO_POWERPC_LINUX_VALID_REVISION;
			}
		}
	}
}
