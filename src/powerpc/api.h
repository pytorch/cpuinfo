#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>

enum cpuinfo_powerpc_chipset_vendor {
	cpuinfo_powerpc_chipset_vendor_unknown = 0,
	cpuinfo_powerpc_chipset_vendor_ibm
};


enum cpuinfo_powerpc_chipset_model {
    cpuinfo_powerpc_chipset_model_unknown = 0,
    cpuinfo_powerpc_chipset_model_power7_arch,
	cpuinfo_powerpc_chipset_model_power8_arch,
	cpuinfo_powerpc_chipset_model_power9_arch,
	cpuinfo_powerpc_chipset_model_power7_raw,
	cpuinfo_powerpc_chipset_model_power7p_raw,
	cpuinfo_powerpc_chipset_model_power8e_raw,
	cpuinfo_powerpc_chipset_model_power8nvl_raw,
	cpuinfo_powerpc_chipset_model_power8_raw,
	cpuinfo_powerpc_chipset_model_power8dd1_raw,
	cpuinfo_powerpc_chipset_model_power9_raw,
	cpuinfo_powerpc_chipset_model_power9dd1_raw,
	cpuinfo_powerpc_chipset_model_power10_arch,
	cpuinfo_powerpc_chipset_model_power10_raw,
};

struct cpuinfo_powerpc_chipset {
	enum cpuinfo_powerpc_chipset_vendor vendor;
	enum cpuinfo_powerpc_chipset_model model;
};

void cpuinfo_powerpc_decode_vendor_uarch(
	uint32_t pvr,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]);

void cpuinfo_powerpc_chipset_decode(
	uint32_t pvr,
	struct cpuinfo_powerpc_chipset chipset[restrict static 1]);

void cpuinfo_powerpc_chipset_to_string(
    const struct cpuinfo_powerpc_chipset chipset[restrict static 1],
    char name[restrict static CPUINFO_PACKAGE_NAME_MAX]);

void cpuinfo_powerpc_decode_cache(
        uint32_t smt_id,
        struct cpuinfo_cache l1i[restrict static 1],
        struct cpuinfo_cache l1d[restrict static 1],
        struct cpuinfo_cache l2[restrict static 1],
        struct cpuinfo_cache l3[restrict static 1]);
