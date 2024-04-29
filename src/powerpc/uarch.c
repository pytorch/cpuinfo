#include <stdint.h>

#include <powerpc/api.h>
#include <cpuinfo/log.h>

#define REV_PVR_MASK 0xffff0000

void cpuinfo_powerpc_decode_vendor_uarch(
		uint32_t pvr,
		enum cpuinfo_vendor vendor[restrict static 1],
		enum cpuinfo_uarch uarch[restrict static 1]) {
	uint16_t rev = ((pvr & REV_PVR_MASK) >> 16);
	switch(rev) {
		case 0x3f:
			*uarch = cpuinfo_uarch_power7;
			*vendor = cpuinfo_vendor_ibm;
			break;
		case 0x4a:
			*uarch = cpuinfo_uarch_power7p;
			*vendor = cpuinfo_vendor_ibm;
			break;
		case 0x4b:
			*uarch = cpuinfo_uarch_power8e;
			*vendor = cpuinfo_vendor_ibm;
			break;
		case 0x4c:
			*uarch = cpuinfo_uarch_power8nvl;
			*vendor = cpuinfo_vendor_ibm;
			break;
		case 0x4d:
			*uarch = cpuinfo_uarch_power8e;
			*vendor = cpuinfo_vendor_ibm;
			break;
		case 0x4e:
			*uarch = cpuinfo_uarch_power9;
			*vendor = cpuinfo_vendor_ibm;
			break;
		case 0x80:
			*uarch = cpuinfo_uarch_power10;
			*vendor = cpuinfo_vendor_ibm;
			break;
		default:
			*uarch = cpuinfo_uarch_unknown;
			*vendor = cpuinfo_vendor_unknown;
			cpuinfo_log_warning("unknown processor revision 0x%x", rev);
	}
}
