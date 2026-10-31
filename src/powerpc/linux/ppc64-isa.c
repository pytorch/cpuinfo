#include <sys/auxv.h>

#if CPUINFO_MOCK
#include <cpuinfo-mock.h>
#endif
#include <powerpc/linux/api.h>

#if CPUINFO_MOCK
static uint64_t mock_hwcap = 0;
void cpuinfo_set_hwcap(uint64_t hwcap) {
	mock_hwcap = hwcap;
}

static uint64_t mock_hwcap2 = 0;
void cpuinfo_set_hwcap2(uint64_t hwcap2) {
	mock_hwcap2 = hwcap2;
}
#endif

/**
 * arch/powerpc/include/uapi/asm/cputable.h
 *
 * This must be kept in sync with the upstream kernel header.
 */
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_ALTIVEC UINT32_C(0x10000000)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_DFP UINT32_C(0x00000400)
#define CPUINFO_POWERPC_LINUX_FEATURE_ARCH_2_06 UINT32_C(0x00000100)
#define CPUINFO_POWERPC_LINUX_FEATURE_HAS_VSX UINT32_C(0x00000080)

#define CPUINFO_POWERPC_LINUX_FEATURE2_ARCH_2_07 UINT32_C(0x80000000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_HTM UINT32_C(0x40000000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_VEC_CRYPTO UINT32_C(0x02000000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_ARCH_3_00 UINT32_C(0x00800000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_HAS_IEEE128 UINT32_C(0x00400000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_DARN UINT32_C(0x00200000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_ARCH_3_1 UINT32_C(0x00040000)
#define CPUINFO_POWERPC_LINUX_FEATURE2_MMA UINT32_C(0x00020000)

void cpuinfo_powerpc_linux_decode_isa_from_hwcap(struct cpuinfo_powerpc_isa isa[restrict static 1]) {
#if CPUINFO_MOCK
	const uint32_t hwcap = (uint32_t)mock_hwcap;
	const uint32_t hwcap2 = (uint32_t)mock_hwcap2;
#else
	const uint32_t hwcap = (uint32_t)getauxval(AT_HWCAP);
	const uint32_t hwcap2 = (uint32_t)getauxval(AT_HWCAP2);
#endif

	isa->altivec = !!(hwcap & CPUINFO_POWERPC_LINUX_FEATURE_HAS_ALTIVEC);
	isa->vsx = !!(hwcap & CPUINFO_POWERPC_LINUX_FEATURE_HAS_VSX);
	isa->dfp = !!(hwcap & CPUINFO_POWERPC_LINUX_FEATURE_HAS_DFP);
	isa->isa_v2_06 = !!(hwcap & CPUINFO_POWERPC_LINUX_FEATURE_ARCH_2_06);

	isa->isa_v2_07 = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_ARCH_2_07);
	isa->isa_v3_00 = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_ARCH_3_00);
	isa->isa_v3_1 = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_ARCH_3_1);
	isa->htm = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_HTM);
	isa->vec_crypto = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_VEC_CRYPTO);
	isa->ieee128 = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_HAS_IEEE128);
	isa->darn = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_DARN);
	isa->mma = !!(hwcap2 & CPUINFO_POWERPC_LINUX_FEATURE2_MMA);
}
