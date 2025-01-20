#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>
#include <loongarch/cpucfg.h>
#include <loongarch/api.h>
#include <linux/api.h>

/* No hard limit in the kernel, maximum length observed on non-rogue kernels is 64 */
#define CPUINFO_HARDWARE_VALUE_MAX 64
/* No hard limit in the kernel, maximum length on Raspberry Pi is 8. Add 1 symbol to detect overly large revision strings */
#define CPUINFO_REVISION_VALUE_MAX 9

#if CPUINFO_ARCH_LOONGARCH64
/* Linux: arch/loongarch/include/uapi/asm/hwcap.h */
#define CPUINFO_LOONGARCH_LINUX_FEATURE_CPUCFG		UINT32_C(0x00000001)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LAM		UINT32_C(0x00000002)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_UAL		UINT32_C(0x00000004)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_FPU		UINT32_C(0x00000008)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LSX		UINT32_C(0x00000010)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LASX		UINT32_C(0x00000020)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_CRC32		UINT32_C(0x00000040)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_COMPLEX		UINT32_C(0x00000080)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_CRYPTO		UINT32_C(0x00000100)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LVZ		UINT32_C(0x00000200)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_X86		UINT32_C(0x00000400)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_ARM		UINT32_C(0x00000800)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_MIPS	UINT32_C(0x00001000)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_PTW		UINT32_C(0x00002000)
#define CPUINFO_LOONGARCH_LINUX_FEATURE_LSPW		UINT32_C(0x00004000)
#endif

#define CPUINFO_LOONGARCH_LINUX_VALID_COMPANYID UINT32_C(0x00010000)
#define CPUINFO_LOONGARCH_LINUX_VALID_PROCESSORID  UINT32_C(0x00020000)
#define CPUINFO_LOONGARCH_LINUX_VALID_REVISION      UINT32_C(0x00040000)
#define CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR    UINT32_C(0x00200000)
#define CPUINFO_LOONGARCH_LINUX_VALID_FEATURES     UINT32_C(0x00400000)
#define CPUINFO_LOONGARCH_LINUX_VALID_INFO          UINT32_C(0x007F0000)
#define CPUINFO_LOONGARCH_LINUX_VALID_CPUCFG          UINT32_C(0x003F0000)

struct cpuinfo_loongarch_linux_processor {
	uint32_t architecture_version;
	uint32_t features;
	uint32_t cpucfg_id;
	enum cpuinfo_vendor vendor;
	enum cpuinfo_uarch uarch;
	uint32_t uarch_index;
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

	/** Linux processor ID */
	uint32_t system_processor_id;
	uint32_t flags;
};


CPUINFO_INTERNAL bool cpuinfo_loongarch_linux_parse_proc_cpuinfo(
	char hardware[restrict static CPUINFO_HARDWARE_VALUE_MAX],
	uint32_t max_processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors_count]);

#if CPUINFO_ARCH_LOONGARCH64
	CPUINFO_INTERNAL void cpuinfo_loongarch64_linux_decode_isa_from_hwcap(
		struct cpuinfo_loongarch_isa isa[restrict static 1]);
#endif

CPUINFO_INTERNAL struct cpuinfo_loongarch_chipset
	cpuinfo_loongarch_linux_decode_chipset(
		const char hardware[restrict static CPUINFO_HARDWARE_VALUE_MAX]);


CPUINFO_INTERNAL struct cpuinfo_loongarch_chipset
	cpuinfo_loongarch_linux_decode_chipset_from_proc_cpuinfo_hardware(
		const char proc_cpuinfo_hardware[restrict static CPUINFO_HARDWARE_VALUE_MAX],
		bool is_loongson);


CPUINFO_INTERNAL bool cpuinfo_loongarch_linux_detect_core_clusters_by_heuristic(
	uint32_t usable_processors,
	uint32_t max_processors,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors]);

CPUINFO_INTERNAL void cpuinfo_loongarch_linux_detect_core_clusters_by_sequential_scan(
	uint32_t max_processors,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors]);

CPUINFO_INTERNAL void cpuinfo_loongarch_linux_count_cluster_processors(
	uint32_t max_processors,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors]);

CPUINFO_INTERNAL uint32_t cpuinfo_loongarch_linux_detect_cluster_cpucfg(
	const struct cpuinfo_loongarch_chipset chipset[restrict static 1],
	uint32_t max_processors,
	uint32_t usable_processors,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors]);

extern CPUINFO_INTERNAL const uint32_t* cpuinfo_linux_cpu_to_uarch_index_map;
extern CPUINFO_INTERNAL uint32_t cpuinfo_linux_cpu_to_uarch_index_map_entries;
