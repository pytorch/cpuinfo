#pragma once

#include <cpuinfo.h>
#include <cpuinfo/common.h>

/**
 * Definition of a RISC-V Linux processor. It is composed of the base processor
 * definition in "include/cpuinfo.h" and flags specific to RISC-V Linux
 * implementations.
 */
struct cpuinfo_riscv_linux_processor {
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
	 * package.
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
};

/**
 * Reads AT_HWCAP from `getauxval` and populates the cpuinfo_riscv_isa
 * structure.
 *
 * @param[isa] - Reference to cpuinfo_riscv_isa structure to populate.
 */
CPUINFO_INTERNAL void cpuinfo_riscv_linux_decode_isa_from_hwcap(struct cpuinfo_riscv_isa isa[restrict static 1]);

/**
 * Reads `sys_riscv_hwprobe` and determines the processor vendor and
 * micro-architecture.
 *
 * @param[processor] - The Linux ID of the target processor.
 * @param[vendor] - Reference to the cpuinfo_vendor to populate.
 * @param[uarch] - Reference to the cpuinfo_uarch to populate.
 * @param[isa] - Reference to the cpuinfo_riscv_isa to populate isa extensions.
 */
CPUINFO_INTERNAL void cpuinfo_riscv_linux_decode_vendor_uarch_from_hwprobe(
	uint32_t processor,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1],
	struct cpuinfo_riscv_isa isa[restrict static 1]);

/**
 * Reads sysfs cache topology for a given CPU and populates cache structures.
 * Returns true if any cache information was found.
 */
CPUINFO_INTERNAL bool cpuinfo_riscv_linux_parse_cache_from_sysfs(
	uint32_t cpu_id,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1]);

/**
 * Information about cache sharing topology for a given cache level.
 */
struct cpuinfo_riscv_cache_sharing_info {
	uint32_t min_cpu;
	uint32_t cpu_count;
};

/**
 * Gets cache sharing information for the cache at the given level.
 * Returns true if the cache level exists for the given CPU.
 */
CPUINFO_INTERNAL bool cpuinfo_riscv_linux_get_cache_sharing(
	uint32_t cpu_id,
	uint32_t cache_level,
	struct cpuinfo_riscv_cache_sharing_info sharing[restrict static 1]);

/* Used to determine which uarch is associated with the current thread. */
extern CPUINFO_INTERNAL const uint32_t* cpuinfo_linux_cpu_to_uarch_index_map;
