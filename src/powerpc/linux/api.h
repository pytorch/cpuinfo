

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>
#include <powerpc/api.h>
#include <linux/api.h>

/* No hard limit in the kernel, maximum length observed on non-rogue kernels is 64 */
#define CPUINFO_HARDWARE_VALUE_MAX 64

/* from /arch/powerpc/kernel/cputable.c */
#define CPUINFO_POWERPC_LINUX_FEATURE_64                                UINT32_C(0x40000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_ALTIVEC               UINT32_C(0x10000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_FPU                   UINT32_C(0x08000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_MMU                   UINT32_C(0x04000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_NO_TB                             UINT32_C(0x00100000)
#define CPUINFO_POWERPC_LINUX_FEATURE_SMT                               UINT32_C(0x00004000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_DFP                   UINT32_C(0x00000400)
#define CPUINFO_POWERPC_LINUX_FEATURE_POWER6_EXT                UINT32_C(0x00000200)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_VSX                   UINT32_C(0x00000080)
#define CPUINFO_POWERPC_LINUX_FEATURE_TRUE_LE                   UINT32_C(0x00000002)
#define CPUINFO_POWERPC_LINUX_FEATURE_PPC_LE                    UINT32_C(0x00000001
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_2_07                 UINT32_C(0x80000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HTM                               UINT32_C(0x40000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_DSCR                              UINT32_C(0x20000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_EBB                               UINT32_C(0x10000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_ISEL                              UINT32_C(0x08000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_TAR                               UINT32_C(0x04000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_VEC_CRYPTO                UINT32_C(0x02000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HTM_NOSC                  UINT32_C(0x01000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_00                 UINT32_C(0x00800000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_IEEE128               UINT32_C(0x00400000)
#define CPUINFO_POWERPC_LINUX_FEATURE_DARN                              UINT32_C(0x00200000)
#define CPUINFO_POWERPC_LINUX_FEATURE_SCV                               UINT32_C(0x00100000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HTM_NO_SUSPEND    UINT32_C(0x00080000)
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_1                  UINT32_C(0x00040000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_MMA                   UINT32_C(0x00020000)

#define CPUINFO_POWERPC_LINUX_VALID_ARCHITECTURE UINT32_C(0x00010000)
#define CPUINFO_POWERPC_LINUX_VALID_IMPLEMENTER  UINT32_C(0x00020000)
#define CPUINFO_POWERPC_LINUX_VALID_REVISION     UINT32_C(0x00040000)
#define CPUINFO_POWERPC_LINUX_VALID_PROCESSOR    UINT32_C(0x00080000)
#define CPUINFO_POWERPC_LINUX_VALID_VERSION      UINT32_C(0x00100000)
#define CPUINFO_POWERPC_LINUX_VALID_FEATURES     UINT32_C(0x00200000)
#define CPUINFO_POWERPC_LINUX_VALID_PVR          UINT32_C(0x001F0000)

/**
 * Definition of a powerpc64 Linux processor. It is composed of the base processor
 * definition in "include/cpuinfo.h" and flags specific to powerpc64 Linux
 * implementations.
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
 * Reads AT_HWCAP from `getauxval` and populates the cpuinfo_powerpc_isa
 * structure.
 *
 * @param[isa] - Reference to cpuinfo_powerpc_isa structure to populate.
 */
void cpuinfo_powerpc_linux_hwcap_from_getauxval(uint32_t isa_feature[]);
CPUINFO_INTERNAL void cpuinfo_ppc64_linux_decode_isa_from_hwcap(uint32_t hwcap, uint32_t hwcap2, struct cpuinfo_powerpc_isa isa[restrict static 1]);

bool cpuinfo_powerpc_linux_parse_proc_cpuinfo(
		        uint32_t max_processors_count,struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors_count]);

/* Used to determine which uarch is associated with the current thread. */
extern CPUINFO_INTERNAL const uint32_t* cpuinfo_linux_cpu_to_uarch_index_map;
