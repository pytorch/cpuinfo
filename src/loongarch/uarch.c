#include <stdint.h>

#include <loongarch/api.h>
#include <loongarch/prid.h>
#include <cpuinfo/log.h>

static void cpuinfo_loongarch_decode_vendor(
	uint32_t companyID,
	enum cpuinfo_vendor vendor[restrict static 1])
{
	switch (companyID) {
		case prid_company_loongson:
			*vendor = cpuinfo_vendor_loongson;
			break;
		default:
			*vendor = cpuinfo_vendor_unknown;
			break;
	}
}

static void cpuinfo_loongarch_decode_uarch(
	uint32_t seriesID,
	enum cpuinfo_uarch uarch[restrict static 1])
{
	switch (seriesID) {
		case prid_series_la264:
			*uarch = cpuinfo_uarch_LA264;
			break;
		case prid_series_la364:
			*uarch = cpuinfo_uarch_LA364;
			break;
		case prid_series_la464:
			*uarch = cpuinfo_uarch_LA464;
			break;
		case prid_series_la664:
			*uarch = cpuinfo_uarch_LA664;
			break;
		default:
			*uarch = cpuinfo_uarch_unknown;
			break;
	}
}

void cpuinfo_loongarch_decode_vendor_uarch(
	uint32_t prid,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1])
{
	cpuinfo_loongarch_decode_vendor(
		prid_get_companyID(prid), vendor);
	cpuinfo_loongarch_decode_uarch(
		prid_get_seriesID(prid), uarch);
}
