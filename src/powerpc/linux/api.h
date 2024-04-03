#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <powerpc/api.h>
#include <linux/api.h>

/* No hard limit in the kernel, maximum length observed on non-rogue kernels is 64 */
#define CPUINFO_HARDWARE_VALUE_MAX 64

/* from /arch/powerpc/kernel/cputable.c */
#define CPUINFO_POWERPC_LINUX_FEATURE_64				UINT32_C(0x40000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_ALTIVEC		UINT32_C(0x10000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_FPU			UINT32_C(0x08000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_MMU			UINT32_C(0x04000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_NO_TB				UINT32_C(0x00100000)
#define CPUINFO_POWERPC_LINUX_FEATURE_SMT				UINT32_C(0x00004000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_DFP			UINT32_C(0x00000400)
#define CPUINFO_POWERPC_LINUX_FEATURE_POWER6_EXT		UINT32_C(0x00000200)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_VSX			UINT32_C(0x00000080)
#define CPUINFO_POWERPC_LINUX_FEATURE_TRUE_LE			UINT32_C(0x00000002)
#define CPUINFO_POWERPC_LINUX_FEATURE_PPC_LE			UINT32_C(0x00000001
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_2_07			UINT32_C(0x80000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HTM				UINT32_C(0x40000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_DSCR				UINT32_C(0x20000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_EBB				UINT32_C(0x10000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_ISEL				UINT32_C(0x08000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_TAR				UINT32_C(0x04000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_VEC_CRYPTO		UINT32_C(0x02000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HTM_NOSC			UINT32_C(0x01000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_00			UINT32_C(0x00800000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_IEEE128		UINT32_C(0x00400000)
#define CPUINFO_POWERPC_LINUX_FEATURE_DARN				UINT32_C(0x00200000)
#define CPUINFO_POWERPC_LINUX_FEATURE_SCV				UINT32_C(0x00100000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HTM_NO_SUSPEND	UINT32_C(0x00080000)
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_3_1			UINT32_C(0x00040000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_MMA                   UINT32_C(0x00020000)

#define CPUINFO_POWERPC_LINUX_VALID_ARCHITECTURE UINT32_C(0x00010000)
#define CPUINFO_POWERPC_LINUX_VALID_IMPLEMENTER  UINT32_C(0x00020000)
#define CPUINFO_POWERPC_LINUX_VALID_REVISION     UINT32_C(0x00040000)
#define CPUINFO_POWERPC_LINUX_VALID_PROCESSOR    UINT32_C(0x00080000)
#define CPUINFO_POWERPC_LINUX_VALID_VERSION      UINT32_C(0x00100000)
#define CPUINFO_POWERPC_LINUX_VALID_FEATURES     UINT32_C(0x00200000)
#define CPUINFO_POWERPC_LINUX_VALID_PVR          UINT32_C(0x001F0000)

struct cpuinfo_powerpc_linux_proc_cpuinfo_cache {
	uint32_t i_size;
	uint32_t i_assoc;
	uint32_t i_line_length;
	uint32_t i_sets;
	uint32_t d_size;
	uint32_t d_assoc;
	uint32_t d_line_length;
	uint32_t d_sets;
};

struct cpuinfo_powerpc_linux_processor {
	uint32_t architecture_version;
	uint32_t architecture_flags;
	struct cpuinfo_powerpc_linux_proc_cpuinfo_cache proc_cpuinfo_cache;
	uint32_t features;

	/**
	 * Main PVR Register value.
	 */
	uint32_t pvr;
	enum cpuinfo_vendor vendor;
	enum cpuinfo_uarch uarch;
	/**
	 * ID of the physical package which includes this logical processor.
	 * The value is parsed from /sys/devices/system/cpu/cpu<N>/topology/physical_package_id
	 */
	uint32_t package_id;
	/**
	 * Minimum processor ID on the package which includes this logical processor.
	 * This value can serve as an ID for the cluster of logical processors: it is the
	 * same for all logical processors on the same package.
	 */
	uint32_t package_leader_id;
	/**
	 * Number of logical processors in the package.
	 */
	uint32_t package_processor_count;
	/**
	 * Maximum frequency, in kHZ.
	 * The value is parsed from /sys/devices/system/cpu/cpu<N>/cpufreq/cpuinfo_max_freq
	 * If failed to read or parse the file, the value is 0.
	 */
	uint32_t max_frequency;
	/**
	 * Minimum frequency, in kHZ.
	 * The value is parsed from /sys/devices/system/cpu/cpu<N>/cpufreq/cpuinfo_min_freq
	 * If failed to read or parse the file, the value is 0.
	 */
	uint32_t min_frequency;
	/** Linux processor ID */
	uint32_t system_processor_id;
	uint32_t flags;

	bool disabled;
};

void cpuinfo_powerpc_linux_hwcap_from_getauxval(uint32_t isa_feature[]);

void cpuinfo_ppc64_linux_decode_isa_from_proc_cpuinfo(
		uint32_t features, uint32_t features2,
		struct cpuinfo_powerpc_isa isa[restrict static 1]);

bool cpuinfo_powerpc_linux_parse_proc_cpuinfo(
        char hardware[restrict static CPUINFO_HARDWARE_VALUE_MAX],
        uint32_t max_processors_count,
        struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors_count]);

void cpuinfo_powerpc_linux_count_cluster_processors(
		uint32_t max_processors,
		struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors]);

uint32_t cpuinfo_powerpc_linux_detect_cluster_pvr(
		uint32_t max_processors,
		uint32_t usable_processors,
		struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors]);

void cpuinfo_powerpc_linux_detect_core_clusters_by_sequential_scan(
	uint32_t max_processors,
	struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors]);
