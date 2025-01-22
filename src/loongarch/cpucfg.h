#pragma once
#include <stdint.h>

#if defined(__GNUC__)
#include <larchintrin.h>
#endif

enum cpucfg_regs {
	CPUCFG_REG_PRID = 0,
	CPUCFG_REG_CACHE_BASEINFO = 0x10,
	CPUCFG_REG_CACHE_L1_IU,
	CPUCFG_REG_CACHE_L1_D,
	CPUCFG_REG_CACHE_L2_IU,
	CPUCFG_REG_CACHE_L3_IU,
};

static inline bool cpucfg(uint32_t reg, uint32_t *value) {
#if defined(__GNUC__)
	*value = __cpucfg(reg);
	return true;
#else
	return false;
#endif
}

enum cache_baseinfo_bit {
	L1_IU_Present = 0,
	L1_IU_Unify,
	L1_D_Present,
	L2_IU_Present,
	L2_IU_Unify,
	L2_IU_Private,
	L2_IU_Inclusive,
	L2_D_Present,
	L2_D_Private,
	L2_D_Inclusive,
	L3_IU_Present,
	L3_IU_Unify,
	L3_IU_Private,
	L3_IU_Inclusive,
	L3_D_Present,
	L3_D_Private,
	L3_D_Inclusive,
};

#define CACHE_WAYS_OFFSET	0
#define CACHE_SETS_OFFSET	16
#define CACHE_LSIZE_OFFSET	24
#define CACHE_WAYS_MASK		UINT32_C(0x0000FFFF)
#define CACHE_SETS_MASK		UINT32_C(0x00FF0000)
#define CACHE_LSIZE_MASK	UINT32_C(0x7F000000)
