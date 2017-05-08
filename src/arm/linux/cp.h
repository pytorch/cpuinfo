#include <stdint.h>


static inline uint32_t read_fpsid(void) {
	uint32_t fpsid;
	__asm__ __volatile__("MRC p10, 0x7, %[fpsid], cr0, cr0, 0" : [fpsid] "=r" (fpsid));
	return fpsid;
}

static inline uint32_t read_mvfr0(void) {
	uint32_t mvfr0;
	__asm__ __volatile__("MRC p10, 0x7, %[mvfr0], cr7, cr0, 0" : [mvfr0] "=r" (mvfr0));
	return mvfr0;
}

static inline uint32_t read_wcid(void) {
	uint32_t wcid;
	__asm__ __volatile__("MRC p1, 0, %[wcid], c0, c0" : [wcid] "=r" (wcid));
	return wcid;
}
