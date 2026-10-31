#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>
#include <powerpc/api.h>

/**
 * A cache of a logical processor, as described by sysfs.
 */
struct cpuinfo_powerpc_linux_cache {
	/** Size in bytes, or 0 if the processor has no such cache. */
	uint32_t size;
	/** Associativity, or 0 if sysfs does not report it. */
	uint32_t associativity;
	/** Line size in bytes, or 0 if sysfs does not report it. */
	uint32_t line_size;
	/**
	 * Minimum ID of the valid logical processors that share this cache.
	 * This value serves as an ID for the cache.
	 */
	uint32_t leader_id;
	/** Number of valid logical processors that share this cache. */
	uint32_t processor_count;
};

/**
 * Definition of a PowerPC Linux processor. It is composed of the base
 * processor definition in "include/cpuinfo.h" and flags specific to PowerPC
 * Linux implementations.
 */
struct cpuinfo_powerpc_linux_processor {
	/* Public ABI cpuinfo structures. */
	struct cpuinfo_processor processor;
	struct cpuinfo_core core;
	struct cpuinfo_cluster cluster;
	struct cpuinfo_package package;

	/**
	 * Linux-specific flags for the logical processor:
	 * - Bit field that can be masked with CPUINFO_LINUX_FLAG_*.
	 */
	uint32_t flags;

	/**
	 * Minimum processor ID on the cluster which includes this logical
	 * processor. This value can serve as an ID for the cluster of logical
	 * processors: it is the same for all logical processors on the same
	 * cluster.
	 */
	uint32_t cluster_leader_id;

	/**
	 * Minimum processor ID on the core which includes this logical
	 * processor. This value can serve as an ID for the core of logical
	 * processors: it is the same for all logical processors on the same
	 * core.
	 */
	uint32_t core_leader_id;

	/**
	 * Minimum processor ID on the package which includes this logical
	 * processor. This value can serve as an ID for the package of logical
	 * processors: it is the same for all logical processors on the same
	 * package.
	 */
	uint32_t package_leader_id;

	/** Value of the Processor Version Register, from /proc/cpuinfo. */
	uint32_t pvr;

	/** Name of the processor, from the "cpu" line of /proc/cpuinfo. */
	char name[CPUINFO_PACKAGE_NAME_MAX];

	/** Caches of the processor, from sysfs. */
	struct cpuinfo_powerpc_linux_cache caches[cpuinfo_powerpc_cache_level_max];
};

/**
 * Parses /proc/cpuinfo for the PVR and name of each processor. Processors
 * whose PVR was found are marked with CPUINFO_LINUX_FLAG_PROC_CPUINFO.
 *
 * @param[max_processors_count] - The number of entries in processors.
 * @param[processors] - The processors, indexed by Linux ID.
 *
 * @returns true if /proc/cpuinfo was parsed, false otherwise.
 */
CPUINFO_INTERNAL bool cpuinfo_powerpc_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_powerpc_linux_processor processors[restrict static 1]);

/**
 * Reads the caches of a processor from sysfs. Only valid processors are
 * counted as sharing a cache.
 *
 * @param[max_processors_count] - The number of entries in processors.
 * @param[processor] - The Linux ID of the processor.
 * @param[processors] - The processors, indexed by Linux ID.
 */
CPUINFO_INTERNAL void cpuinfo_powerpc_linux_detect_caches(
	uint32_t max_processors_count,
	uint32_t processor,
	struct cpuinfo_powerpc_linux_processor processors[restrict static 1]);

/**
 * Reads AT_HWCAP and AT_HWCAP2 from `getauxval` and populates the
 * cpuinfo_powerpc_isa structure.
 *
 * @param[isa] - Reference to cpuinfo_powerpc_isa structure to populate.
 */
CPUINFO_INTERNAL void cpuinfo_powerpc_linux_decode_isa_from_hwcap(struct cpuinfo_powerpc_isa isa[restrict static 1]);

/* Used to determine which uarch is associated with the current thread. */
extern CPUINFO_INTERNAL const uint32_t* cpuinfo_linux_cpu_to_uarch_index_map;
