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

static inline bool bitmask_all(uint32_t bitfield, uint32_t mask) {
	return (bitfield & mask) == mask;
}


/*
 * Counts the number of logical processors in each core cluster.
 * This function should be called after all processors are assigned to core clusters.
 *
 * @param max_processors - number of elements in the @p processors array.
 * @param[in,out] processors - processor descriptors with pre-parsed POSSIBLE and PRESENT flags,
 *                             and decoded core cluster (package_leader_id) information.
 *                             The function expects the value of processors[i].package_processor_count to be zero.
 *                             Upon return, processors[i].package_processor_count will contain the number of logical
 *                             processors in the respective core cluster.
 */
void cpuinfo_loongarch_linux_count_cluster_processors(
	uint32_t max_processors,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors])
{
	/* First pass: accumulate the number of processors at the group leader's package_processor_count */
	for (uint32_t i = 0; i < max_processors; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			const uint32_t package_leader_id = processors[i].package_leader_id;
			processors[package_leader_id].package_processor_count += 1;
		}
	}
	/* Second pass: copy the package_processor_count from the group leader processor */
	for (uint32_t i = 0; i < max_processors; i++) {
		if (bitmask_all(processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			const uint32_t package_leader_id = processors[i].package_leader_id;
			processors[i].package_processor_count = processors[package_leader_id].package_processor_count;
		}
	}
}
