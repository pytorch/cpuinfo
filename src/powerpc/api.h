#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>

enum cpuinfo_powerpc_chipset_vendor { cpuinfo_powerpc_chipset_vendor_unknown = 0, cpuinfo_powerpc_chipset_vendor_ibm };
void cpuinfo_powerpc_decode_vendor_uarch(
	uint32_t vendor_id,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]);

void cpuinfo_powerpc_decode_cache(
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1]);
