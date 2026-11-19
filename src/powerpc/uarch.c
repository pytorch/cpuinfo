#include <stdint.h>

#include <cpuinfo/log.h>
#include <powerpc/api.h>

void cpuinfo_powerpc_decode_vendor_uarch(
	uint32_t pvr,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]) {
	switch (pvr >> 16) {
		case cpuinfo_powerpc_pvr_version_power8e:
		case cpuinfo_powerpc_pvr_version_power8nvl:
		case cpuinfo_powerpc_pvr_version_power8:
			*vendor = cpuinfo_vendor_ibm;
			*uarch = cpuinfo_uarch_power8;
			break;
		case cpuinfo_powerpc_pvr_version_power9:
			*vendor = cpuinfo_vendor_ibm;
			*uarch = cpuinfo_uarch_power9;
			break;
		case cpuinfo_powerpc_pvr_version_power10:
			*vendor = cpuinfo_vendor_ibm;
			*uarch = cpuinfo_uarch_power10;
			break;
		case cpuinfo_powerpc_pvr_version_power11:
			*vendor = cpuinfo_vendor_ibm;
			*uarch = cpuinfo_uarch_power11;
			break;
		default:
			*vendor = cpuinfo_vendor_unknown;
			*uarch = cpuinfo_uarch_unknown;
			cpuinfo_log_warning("unknown PVR: 0x%08" PRIx32, pvr);
			break;
	}
}
