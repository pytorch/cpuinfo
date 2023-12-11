#include <string.h>
#include <sys/auxv.h>

#include <riscv/linux/api.h>

#if CPUINFO_MOCK
#include <cpuinfo-mock.h>
#endif  // CPUINFO_MOCK

/**
 * arch/riscv/include/uapi/asm/hwcap.h
 *
 * This must be kept in sync with the upstream kernel header.
 */
#define COMPAT_HWCAP_ISA_I	(1 << ('I' - 'A'))
#define COMPAT_HWCAP_ISA_M	(1 << ('M' - 'A'))
#define COMPAT_HWCAP_ISA_A	(1 << ('A' - 'A'))
#define COMPAT_HWCAP_ISA_F	(1 << ('F' - 'A'))
#define COMPAT_HWCAP_ISA_D	(1 << ('D' - 'A'))
#define COMPAT_HWCAP_ISA_C	(1 << ('C' - 'A'))
#define COMPAT_HWCAP_ISA_V	(1 << ('V' - 'A'))

#if CPUINFO_MOCK
static uint32_t g_mock_hwcap = 0;
void cpuinfo_set_hwcap(uint32_t hwcap) {
    g_mock_hwcap = hwcap;
}
#endif  // CPUINFO_MOCK

void cpuinfo_riscv_linux_decode_isa_from_hwcap(
		struct cpuinfo_riscv_isa isa[restrict static 1]) {
#if CPUINFO_MOCK
	const uint32_t hwcap = g_mock_hwcap;
#else
	const uint32_t hwcap = getauxval(AT_HWCAP);
#endif  // CPUINFO_MOCK

	if (hwcap & COMPAT_HWCAP_ISA_I) {
		isa->i = true;
	}
	if (hwcap & COMPAT_HWCAP_ISA_M) {
		isa->m = true;
	}
	if (hwcap & COMPAT_HWCAP_ISA_A) {
		isa->a = true;
	}
	if (hwcap & COMPAT_HWCAP_ISA_F) {
		isa->f = true;
	}
	if (hwcap & COMPAT_HWCAP_ISA_D) {
		isa->d = true;
	}
	if (hwcap & COMPAT_HWCAP_ISA_C) {
		isa->c = true;
	}
	if (hwcap & COMPAT_HWCAP_ISA_V) {
		isa->v = true;
	}
}
