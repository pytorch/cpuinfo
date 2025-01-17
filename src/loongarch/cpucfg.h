#pragma once
#include <stdint.h>



#define CPUINFO_LOONGARCH_CPUCFG_COMPANYID_MASK  UINT32_C(0x00FF0000)
#define CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_MASK      UINT32_C(0x0000FF00)
#define CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK UINT32_C(0x000000FF)


#define CPUINFO_LOONGARCH_CPUCFG_COMPANYID_OFFSET 			16
#define CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_OFFSET       	8
#define CPUINFO_LOONGARCH_CPUCFG_REVISION_OFFSET      	0



inline static uint32_t cpucfg_set_companyID(uint32_t cpucfg, uint32_t companyID) {
	return (cpucfg & ~CPUINFO_LOONGARCH_CPUCFG_COMPANYID_MASK) |
		((companyID << CPUINFO_LOONGARCH_CPUCFG_COMPANYID_OFFSET) & CPUINFO_LOONGARCH_CPUCFG_COMPANYID_MASK);
}

inline static uint32_t cpucfg_set_processorID(uint32_t cpucfg, uint32_t processorID) {
	return (cpucfg & ~CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_MASK) |
		((processorID << CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_OFFSET) & CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_MASK);
}

inline static uint32_t cpucfg_set_revision(uint32_t cpucfg, uint32_t revision) {
	return (cpucfg & ~CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK) |
		((revision << CPUINFO_LOONGARCH_CPUCFG_REVISION_OFFSET) & CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK);
}




inline static uint32_t cpucfg_get_companyID(uint32_t cpucfg) {
	return (cpucfg & CPUINFO_LOONGARCH_CPUCFG_COMPANYID_MASK) >> CPUINFO_LOONGARCH_CPUCFG_COMPANYID_OFFSET;
}

inline static uint32_t cpucfg_get_processorID(uint32_t cpucfg) {
	return (cpucfg & CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_MASK) >> CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_OFFSET;
}

inline static uint32_t cpucfg_get_revision(uint32_t cpucfg) {
	return (cpucfg & CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK) >> CPUINFO_LOONGARCH_CPUCFG_REVISION_OFFSET;
}



inline static uint32_t cpucfg_copy_companyID(uint32_t cpucfg, uint32_t other_cpucfg) {
	return (cpucfg & ~CPUINFO_LOONGARCH_CPUCFG_COMPANYID_MASK) | (other_cpucfg & CPUINFO_LOONGARCH_CPUCFG_COMPANYID_MASK);
}

inline static uint32_t cpucfg_copy_processorID(uint32_t cpucfg, uint32_t other_cpucfg) {
	return (cpucfg & ~CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_MASK) | (other_cpucfg & CPUINFO_LOONGARCH_CPUCFG_PROCESSORID_MASK);
}


inline static uint32_t cpucfg_copy_revision(uint32_t cpucfg, uint32_t other_cpucfg) {
	return (cpucfg & ~CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK) | (other_cpucfg & CPUINFO_LOONGARCH_CPUCFG_REVISION_MASK);
}

