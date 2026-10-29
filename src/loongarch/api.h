#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>

CPUINFO_INTERNAL void cpuinfo_loongarch_decode_vendor_uarch(
	uint32_t prid,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]);
