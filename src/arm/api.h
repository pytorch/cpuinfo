#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>


void cpuinfo_arm_decode_vendor_uarch(
	uint32_t cpu_implementer,
	uint32_t cpu_part,
#if CPUINFO_ARCH_ARM
	bool has_vfpv4,
#endif
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]);

void cpuinfo_arm_decode_cache(
	enum cpuinfo_uarch uarch,
	uint32_t uarch_cores,
	uint32_t cpu_part,
	uint32_t arch_version,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1]);
