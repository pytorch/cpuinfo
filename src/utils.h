#pragma once

#include <stdint.h>


inline static uint32_t bit_length(uint32_t n) {
	const uint32_t n_minus_1 = n - 1;
	if (n_minus_1 == 0) {
		return 0;
	} else {
		return 32 - __builtin_clz(n_minus_1);
	}
}
