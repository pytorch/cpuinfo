#pragma once
#include <stdint.h>

#if defined(__GNUC__)
#include <larchintrin.h>
#endif

enum {
	CPUCFG_REG_PRID = 0,
};

static inline bool cpucfg(uint32_t reg, uint32_t *value) {
#if defined(__GNUC__)
	*value = __cpucfg(reg);
	return true;
#else
	return false;
#endif
}
