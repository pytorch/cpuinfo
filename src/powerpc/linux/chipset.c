#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <powerpc/linux/api.h>
#include <cpuinfo.h>
#include <cpuinfo/log.h>

#define POWERPC_PVR_VERSION(x) (x & 0xffff0000) >> 16
#define POWERPC_PVR_REVISION(x) x & 0xffff

void cpuinfo_powerpc_chipset_decode(
	uint32_t pvr,
	struct cpuinfo_powerpc_chipset chipset[restrict static 1])
{

	switch(POWERPC_PVR_VERSION(pvr)) {
		case 0xf0:
			switch(POWERPC_PVR_REVISION(pvr)) {
				case 3:
					chipset->model = cpuinfo_powerpc_chipset_model_power7_arch;
				break;
				case 4:
					chipset->model = cpuinfo_powerpc_chipset_model_power8_arch;
				break;
				case 5:
				chipset->model = cpuinfo_powerpc_chipset_model_power9_arch;
				break;
				case 6:
				chipset->model = cpuinfo_powerpc_chipset_model_power10_arch;
				break;
				default:
					cpuinfo_log_warning("CPU model is unsupported due unknown revision PVR: 0x%x revision: %x", pvr, POWERPC_PVR_REVISION(pvr));
			}
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case  0x3f:
			chipset->model = cpuinfo_powerpc_chipset_model_power7_raw;
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case 0x4a:
			chipset->model = cpuinfo_powerpc_chipset_model_power7p_raw;
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case 0x4b:
			chipset->model = cpuinfo_powerpc_chipset_model_power8e_raw;
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case 0x4c:
			chipset->model = cpuinfo_powerpc_chipset_model_power8nvl_raw;
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case 0x4d:
			if (POWERPC_PVR_REVISION(pvr) == 0x100) {
				chipset->model = cpuinfo_powerpc_chipset_model_power8dd1_raw;
			} else {
				chipset->model = cpuinfo_powerpc_chipset_model_power8_raw;
			}
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case 0x4e:
			if (POWERPC_PVR_REVISION(pvr) == 0x100) {
				chipset->model = cpuinfo_powerpc_chipset_model_power9dd1_raw;
			} else {
				chipset->model = cpuinfo_powerpc_chipset_model_power9_raw;
			}
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		break;
		case 0x80:
			chipset->model = cpuinfo_powerpc_chipset_model_power10_raw;
			chipset->vendor = cpuinfo_powerpc_chipset_vendor_ibm;
		default:
			cpuinfo_log_warning("CPU model is unsupported PVR: 0x%x", pvr);
		break;
		unknown_revision:
			cpuinfo_log_warning("CPU model is unsupported due unknown revision PVR: 0x%x revision: %x", pvr, POWERPC_PVR_REVISION(pvr));
		break;
	}
}

/* Convert chipset name represented by cpuinfo_powepc_chipset structure to a string representation */
void cpuinfo_powerpc_chipset_to_string(
	const struct cpuinfo_powerpc_chipset chipset[restrict static 1],
	char name[restrict static CPUINFO_PACKAGE_NAME_MAX])
{
	const char* vendor_string = "Unknown";
	const char* model_string = "Unknown";

	switch(chipset->vendor) {
		case cpuinfo_powerpc_chipset_vendor_ibm:
			vendor_string = "IBM";
		break;
	}

	switch(chipset->model) {
		case cpuinfo_powerpc_chipset_model_power7_arch:
			model_string = "POWER7 (architected)";
		break;
		case cpuinfo_powerpc_chipset_model_power8_arch:
			model_string = "POWER8 (architected)";
		break;
		case cpuinfo_powerpc_chipset_model_power9_arch:
			model_string = "POWER9 (architected)";
			break;
		case cpuinfo_powerpc_chipset_model_power10_arch:
			model_string = "POWER10 (architected)";
			break;
		case cpuinfo_powerpc_chipset_model_power7_raw:
			model_string = "POWER7 (raw)";
		break;
		case cpuinfo_powerpc_chipset_model_power7p_raw:
			model_string = "POWER7+ (raw)";
		break;
		case cpuinfo_powerpc_chipset_model_power8e_raw:
			model_string = "POWER8E (raw)";
		break;
		case cpuinfo_powerpc_chipset_model_power8nvl_raw:
			model_string = "POWER8NVL (raw)";
		break;
		case cpuinfo_powerpc_chipset_model_power8_raw:
		case cpuinfo_powerpc_chipset_model_power8dd1_raw:
			model_string = "POWER9 (raw)";
		break;
		case cpuinfo_powerpc_chipset_model_power9_raw:
		case cpuinfo_powerpc_chipset_model_power9dd1_raw:
			model_string = "POWER9 (raw)";
			break;
		case cpuinfo_powerpc_chipset_model_power10_raw:
			model_string = "POWER10 (raw)";
			break;
	}

	snprintf(name, CPUINFO_PACKAGE_NAME_MAX, "%s %s", vendor_string, model_string);
}
