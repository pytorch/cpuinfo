#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>
#include <loongarch/prid.h>
#include <loongarch/api.h>
#include <linux/api.h>

/* No hard limit in the kernel, maximum length observed on non-rogue kernels is 64 */
#define CPUINFO_HARDWARE_VALUE_MAX 64

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

#define CPUINFO_LOONGARCH_LINUX_VALID_COMPANYID		UINT32_C(0x00010000)
#define CPUINFO_LOONGARCH_LINUX_VALID_SERIESID		UINT32_C(0x00020000)
#define CPUINFO_LOONGARCH_LINUX_VALID_REVISION		UINT32_C(0x00040000)
#define CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR		UINT32_C(0x00200000)
#define CPUINFO_LOONGARCH_LINUX_VALID_FEATURES		UINT32_C(0x00400000)
#define CPUINFO_LOONGARCH_LINUX_VALID_INFO		UINT32_C(0x007F0000)
#define CPUINFO_LOONGARCH_LINUX_VALID_L1I		UINT32_C(0x01000000)
#define CPUINFO_LOONGARCH_LINUX_VALID_L1D		UINT32_C(0x02000000)
#define CPUINFO_LOONGARCH_LINUX_VALID_L2		UINT32_C(0x04000000)
#define CPUINFO_LOONGARCH_LINUX_VALID_L3		UINT32_C(0x08000000)
#define CPUINFO_LOONGARCH_LINUX_VALID_PRID		(CPUINFO_LOONGARCH_LINUX_VALID_COMPANYID | CPUINFO_LOONGARCH_LINUX_VALID_SERIESID | CPUINFO_LOONGARCH_LINUX_VALID_REVISION)
#define CPUINFO_LOONGARCH_LINUX_VALID_CACHE		(CPUINFO_LOONGARCH_LINUX_VALID_L1I | CPUINFO_LOONGARCH_LINUX_VALID_L1D | CPUINFO_LOONGARCH_LINUX_VALID_L2 | CPUINFO_LOONGARCH_LINUX_VALID_L3)

struct cpuinfo_loongarch_linux_processor {
	uint32_t features;
	uint32_t prid;
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
	/** CoreID */
	uint32_t core_id;
	/** SmtID */
	uint32_t smt_id;

	/** Cache info */
	struct cpuinfo_cache l1i;
	struct cpuinfo_cache l1d;
	struct cpuinfo_cache l2;
	struct cpuinfo_cache l3;

	/** Hardware name */
	char hardware_name[CPUINFO_HARDWARE_VALUE_MAX];

	uint32_t flags;
};


CPUINFO_INTERNAL bool cpuinfo_loongarch_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors_count]);

#if CPUINFO_ARCH_LOONGARCH64
	CPUINFO_INTERNAL void cpuinfo_loongarch64_linux_decode_isa_from_hwcap(
		struct cpuinfo_loongarch_isa isa[restrict static 1]);
#endif

CPUINFO_INTERNAL bool cpuinfo_loongarch_linux_parse_cpu_cache(
	uint32_t max_processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors_count]);

extern CPUINFO_INTERNAL const uint32_t* cpuinfo_linux_cpu_to_uarch_index_map;
extern CPUINFO_INTERNAL uint32_t cpuinfo_linux_cpu_to_uarch_index_map_entries;
