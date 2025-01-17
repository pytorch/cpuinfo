#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>


enum cpuinfo_loongarch_chipset_vendor {
	cpuinfo_loongarch_chipset_vendor_unknown = 0,
	cpuinfo_loongarch_chipset_vendor_Loongson,
	cpuinfo_loongarch_chipset_vendor_max,
};


enum cpuinfo_loongarch_chipset_series {
	cpuinfo_loongarch_chipset_series_unknown = 0,
	cpuinfo_loongarch_chipset_series_3,
	cpuinfo_loongarch_chipset_series_max,
};


#define CPUINFO_LOONGARCH_CHIPSET_SUFFIX_MAX 8

struct cpuinfo_loongarch_chipset {
	enum cpuinfo_loongarch_chipset_vendor vendor;
	enum cpuinfo_loongarch_chipset_series series;
};


#define CPUINFO_LOONGARCH_CHIPSET_NAME_MAX CPUINFO_PACKAGE_NAME_MAX

#ifndef __cplusplus
	CPUINFO_INTERNAL void cpuinfo_loongarch_chipset_to_string(
		const struct cpuinfo_loongarch_chipset chipset[restrict static 1],
		char name[restrict static CPUINFO_LOONGARCH_CHIPSET_NAME_MAX]);

	CPUINFO_INTERNAL void cpuinfo_loongarch_fixup_chipset(
		struct cpuinfo_loongarch_chipset chipset[restrict static 1], uint32_t cores);

	CPUINFO_INTERNAL void cpuinfo_loongarch_decode_vendor_uarch(
		uint32_t cpucfg,
		enum cpuinfo_vendor vendor[restrict static 1],
		enum cpuinfo_uarch uarch[restrict static 1]);

	CPUINFO_INTERNAL void cpuinfo_loongarch_decode_cache(
		enum cpuinfo_uarch uarch,
		uint32_t cluster_cores,
		uint32_t arch_version,
		struct cpuinfo_cache l1i[restrict static 1],
		struct cpuinfo_cache l1d[restrict static 1],
		struct cpuinfo_cache l2[restrict static 1],
		struct cpuinfo_cache l3[restrict static 1]);

	CPUINFO_INTERNAL uint32_t cpuinfo_loongarch_compute_max_cache_size(
		const struct cpuinfo_processor processor[restrict static 1]);
#else /* defined(__cplusplus) */
	CPUINFO_INTERNAL void cpuinfo_loongarch_decode_cache(
		enum cpuinfo_uarch uarch,
		uint32_t cluster_cores,
		uint32_t arch_version,
		struct cpuinfo_cache l1i[1],
		struct cpuinfo_cache l1d[1],
		struct cpuinfo_cache l2[1],
		struct cpuinfo_cache l3[1]);
#endif
