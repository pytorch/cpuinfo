#include <stdint.h>

#include <loongarch/api.h>
#include <loongarch/cpucfg.h>
#include <cpuinfo/log.h>


void cpuinfo_loongarch_decode_vendor_uarch(
	uint32_t cpucfg,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1])
{
	
	switch (cpucfg_get_companyID(cpucfg)) {
		case 0x14:
			*vendor = cpuinfo_vendor_loongson;
			switch(cpucfg_get_processorID(cpucfg)){
				case 0xc0:
					*uarch = cpuinfo_uarch_LA464;
					break;
			}
			break;
		default:
			//not match verify vendor and uarch
			*vendor = cpuinfo_vendor_unknown;
			*uarch = cpuinfo_uarch_unknown;
	}
}
