#pragma once
#include <stdint.h>

/*
 * LoongArch can get PRID by `cpucfg 0` on LoongArch expect LA32R.
 * This file support interface to construct PRID.
 *
 * Linux: arch/loongarch/include/asm/cpu.h
 * As described in LoongArch specs from Loongson Technology, the PRID register
 * (CPUCFG.00) has the following layout:
 *
 * +---------------+----------------+------------+--------------------+
 * | Reserved      | Company ID     | Series ID  |  Product ID        |
 * +---------------+----------------+------------+--------------------+
 *  31           24 23            16 15        12 11                 0
 */

#define CPUINFO_LOONGARCH_PRID_COMPANY_MASK		UINT32_C(0x00FF0000)
#define CPUINFO_LOONGARCH_PRID_SERIES_MASK		UINT32_C(0x0000F000)
#define CPUINFO_LOONGARCH_PRID_PRODUCT_MASK		UINT32_C(0x00000FFF)

#define CPUINFO_LOONGARCH_PRID_COMPANY_OFFSET		16
#define CPUINFO_LOONGARCH_PRID_SERIES_OFFSET		12
#define CPUINFO_LOONGARCH_PRID_PRODUCT_OFFSET		0

#define PRID_COMP_LOONGSON      0x140000

enum prid_company {
	prid_company_loongson = 0x14,	/* Loongson Technology */
};

enum prid_series {
	prid_series_la132 = 0x8,	/* Loongson 32bit */
	prid_series_la264 = 0xa,	/* Loongson 64bit, 2-issue */
	prid_series_la364 = 0xb,	/* Loongson 64bit, 3-issue */
	prid_series_la464 = 0xc,	/* Loongson 64bit, 4-issue */
	prid_series_la664 = 0xd,	/* Loongson 64bit, 6-issue */
};

inline static uint32_t prid_set_companyID(uint32_t prid, uint32_t companyID) {
	return (prid & ~CPUINFO_LOONGARCH_PRID_COMPANY_MASK) |
		((companyID << CPUINFO_LOONGARCH_PRID_COMPANY_OFFSET) & CPUINFO_LOONGARCH_PRID_COMPANY_MASK);
}

inline static uint32_t prid_set_seriesID(uint32_t prid, uint32_t seriesID) {
	return (prid & ~CPUINFO_LOONGARCH_PRID_SERIES_MASK) |
		((seriesID << CPUINFO_LOONGARCH_PRID_SERIES_OFFSET) & CPUINFO_LOONGARCH_PRID_SERIES_MASK);
}

inline static uint32_t prid_set_productID(uint32_t prid, uint32_t productID) {
	return (prid & ~CPUINFO_LOONGARCH_PRID_PRODUCT_MASK) |
		((productID << CPUINFO_LOONGARCH_PRID_PRODUCT_OFFSET) & CPUINFO_LOONGARCH_PRID_PRODUCT_MASK);
}

inline static uint32_t prid_get_companyID(uint32_t prid) {
	return (prid & CPUINFO_LOONGARCH_PRID_COMPANY_MASK) >> CPUINFO_LOONGARCH_PRID_COMPANY_OFFSET;
}

inline static uint32_t prid_get_seriesID(uint32_t prid) {
	return (prid & CPUINFO_LOONGARCH_PRID_SERIES_MASK) >> CPUINFO_LOONGARCH_PRID_SERIES_OFFSET;
}

inline static uint32_t prid_get_productID(uint32_t prid) {
	return (prid & CPUINFO_LOONGARCH_PRID_PRODUCT_MASK) >> CPUINFO_LOONGARCH_PRID_PRODUCT_OFFSET;
}
