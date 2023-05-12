#pragma once

#include <inttypes.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>

#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
    /* arch/riscv/include/uapi/asm/hwcap.h */
	#define CPUINFO_RISCV_LINUX_FEATURE_A                UINT32_C(0x00000001)
	#define CPUINFO_RISCV_LINUX_FEATURE_C                UINT32_C(0x00000004)
	#define CPUINFO_RISCV_LINUX_FEATURE_D                UINT32_C(0x00000008)
	#define CPUINFO_RISCV_LINUX_FEATURE_F                UINT32_C(0x00000020)
	#define CPUINFO_RISCV_LINUX_FEATURE_I                UINT32_C(0x00000100)
	#define CPUINFO_RISCV_LINUX_FEATURE_M                UINT32_C(0x00001000)
	#define CPUINFO_RISCV_LINUX_FEATURE_V                UINT32_C(0x00200000)
#endif

#define CPUINFO_RISCV_LINUX_VALID_ARCHITECTURE           UINT32_C(0x10000000)
#define CPUINFO_RISCV_LINUX_VALID_IMPLEMENTER            UINT32_C(0x20000000)
#define CPUINFO_RISCV_LINUX_VALID_PROCESSOR              UINT32_C(0x40000000)
#define CPUINFO_RISCV_LINUX_VALID_FEATURES               UINT32_C(0x80000000)

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
	 * Minimum processor ID on the cluster which includes this logical processor.
	 * This value can serve as an ID for the cluster of logical processors: it is the
	 * same for all logical processors on the same package.
	 */
	uint32_t cluster_leader_id;

	/**
	 * Minimum processor ID on the core which includes this logical processor.
	 * This value can serve as an ID for the core of logical processors: it
	 * is the same for all logical processors on the same core.
	 */
	uint32_t core_leader_id;

	/**
	 * Minimum processor ID on the package which includes this logical processor.
	 * This value can serve as an ID for the package of logical processors: it
	 * is the same for all logical processors on the same package.
	 */
	uint32_t package_leader_id;

	/* RISC-V ISA extensions supported by this processor */
	uint32_t features;
};

/**
 * Reads AT_HWCAP from `getauxval` and populates the cpuinfo_riscv_isa
 * structure.
 *
 * @param[isa] - Reference to cpuinfo_riscv_isa structure to populate.
 */
CPUINFO_INTERNAL void cpuinfo_riscv_linux_decode_isa_from_hwcap(
	struct cpuinfo_riscv_isa isa[restrict static 1]);

/**
 * Reads `sys_riscv_hwprobe` and determines the processor vendor and
 * micro-architecture.
 *
 * @param[processor] - The Linux ID of the target processor.
 * @param[vendor] - Reference to the cpuinfo_vendor to populate.
 * @param[uarch] - Reference to the cpuinfo_uarch to populate.
 */
CPUINFO_INTERNAL void cpuinfo_riscv_linux_decode_vendor_uarch_from_hwprobe(
	uint32_t processor,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]);

CPUINFO_INTERNAL void cpuinfo_riscv_linux_count_cluster_processors(
	uint32_t max_processors,
	struct cpuinfo_riscv_linux_processor processors[restrict static max_processors]);

CPUINFO_INTERNAL void cpuinfo_riscv_linux_hwcap_from_getauxval(
	uint32_t hwcap[restrict static 1]);

CPUINFO_INTERNAL bool cpuinfo_riscv_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_riscv_linux_processor processors[restrict static max_processors_count]);

/* Used to determine which uarch is associated with the current thread. */
extern CPUINFO_INTERNAL const uint32_t* cpuinfo_linux_cpu_to_uarch_index_map;