#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>

/**
 * Version field (upper 16 bits) of the Processor Version Register (PVR).
 *
 * arch/powerpc/include/asm/reg.h in the Linux kernel.
 */
enum cpuinfo_powerpc_pvr_version {
	cpuinfo_powerpc_pvr_version_power8e = 0x004B,
	cpuinfo_powerpc_pvr_version_power8nvl = 0x004C,
	cpuinfo_powerpc_pvr_version_power8 = 0x004D,
	cpuinfo_powerpc_pvr_version_power9 = 0x004E,
	cpuinfo_powerpc_pvr_version_power10 = 0x0080,
	cpuinfo_powerpc_pvr_version_power11 = 0x0082,
};

/**
 * Cache levels, in the order of cpuinfo's cache arrays.
 */
enum cpuinfo_powerpc_cache_level {
	cpuinfo_powerpc_cache_level_l1i = 0,
	cpuinfo_powerpc_cache_level_l1d,
	cpuinfo_powerpc_cache_level_l2,
	cpuinfo_powerpc_cache_level_l3,
	cpuinfo_powerpc_cache_level_max,
};

/**
 * Geometry of a cache: what the operating system does not reliably report.
 * A value of 0 means unknown.
 */
struct cpuinfo_powerpc_cache_geometry {
	uint32_t associativity;
	uint32_t line_size;
};

/**
 * Decodes the vendor and micro-architecture from the Processor Version
 * Register, regardless of the underlying operating system.
 *
 * @param[pvr] - The value of the Processor Version Register.
 * @param[vendor] - Reference to the cpuinfo_vendor to populate.
 * @param[uarch] - Reference to the cpuinfo_uarch to populate.
 */
CPUINFO_INTERNAL void cpuinfo_powerpc_decode_vendor_uarch(
	uint32_t pvr,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]);

/**
 * Looks up the documented geometry of a cache of a micro-architecture.
 *
 * @param[uarch] - The micro-architecture.
 * @param[level] - The cache level.
 * @param[geometry] - Reference to the geometry to populate, with 0 for the
 *                    values that are not documented.
 */
CPUINFO_INTERNAL void cpuinfo_powerpc_decode_cache_geometry(
	enum cpuinfo_uarch uarch,
	enum cpuinfo_powerpc_cache_level level,
	struct cpuinfo_powerpc_cache_geometry geometry[restrict static 1]);
