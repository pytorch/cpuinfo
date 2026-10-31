#include <stdbool.h>
#include <stdint.h>

#include <powerpc/api.h>

/*
 * The operating system reports the size, level and sharing of each cache
 * reliably, but not its geometry. On bare metal (PowerNV), the firmware
 * describes every POWER9 cache as having 8 "sets" and gives no line size for
 * L2 and L3, so Linux reports a 32-way L1 and a 2048-way L2 with 32-byte
 * lines. Under KVM, only the L1 sizes and line sizes are reported.
 *
 * So the geometry comes from IBM's documentation instead. A value of 0 means
 * that it is not documented, and is taken from the operating system.
 *
 * POWER8 and POWER9: OpenPOWER Foundation, CompArch-MIPS-POWER lectures,
 * chapter 2 part 3 (memory hierarchy), after the POWER9 Processor User's
 * Manual.
 * Power10: "IBM's POWER10 Processor", Hot Chips 32 (L1), and IBM, "IBM AIX on
 * Power10 Performance Topics" (L2).
 */

static const struct cpuinfo_powerpc_cache_geometry power8_caches[cpuinfo_powerpc_cache_level_max] = {
	[cpuinfo_powerpc_cache_level_l1i] = {.associativity = 8},
	[cpuinfo_powerpc_cache_level_l1d] = {.associativity = 8, .line_size = 128},
	[cpuinfo_powerpc_cache_level_l2] = {.associativity = 8, .line_size = 128},
	[cpuinfo_powerpc_cache_level_l3] = {.associativity = 8, .line_size = 128},
};

static const struct cpuinfo_powerpc_cache_geometry power9_caches[cpuinfo_powerpc_cache_level_max] = {
	[cpuinfo_powerpc_cache_level_l1i] = {.associativity = 8, .line_size = 128},
	[cpuinfo_powerpc_cache_level_l1d] = {.associativity = 8, .line_size = 128},
	[cpuinfo_powerpc_cache_level_l2] = {.associativity = 8, .line_size = 128},
	[cpuinfo_powerpc_cache_level_l3] = {.associativity = 20, .line_size = 128},
};

static const struct cpuinfo_powerpc_cache_geometry power10_caches[cpuinfo_powerpc_cache_level_max] = {
	[cpuinfo_powerpc_cache_level_l1i] = {.associativity = 6},
	[cpuinfo_powerpc_cache_level_l1d] = {.associativity = 8},
	[cpuinfo_powerpc_cache_level_l2] = {.associativity = 8},
	[cpuinfo_powerpc_cache_level_l3] = {0},
};

void cpuinfo_powerpc_decode_cache_geometry(
	enum cpuinfo_uarch uarch,
	enum cpuinfo_powerpc_cache_level level,
	struct cpuinfo_powerpc_cache_geometry geometry[restrict static 1]) {
	const struct cpuinfo_powerpc_cache_geometry* caches;
	switch (uarch) {
		case cpuinfo_uarch_power8:
			caches = power8_caches;
			break;
		case cpuinfo_uarch_power9:
			caches = power9_caches;
			break;
		case cpuinfo_uarch_power10:
			caches = power10_caches;
			break;
		default:
			*geometry = (struct cpuinfo_powerpc_cache_geometry){0};
			return;
	}
	*geometry = caches[level];
}
