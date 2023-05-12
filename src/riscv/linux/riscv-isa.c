#include <inttypes.h>
#include <string.h>
#include <sys/auxv.h>

#include <riscv/linux/api.h>

void cpuinfo_riscv_linux_decode_isa_from_hwcap(
		struct cpuinfo_riscv_isa isa[restrict static 1]) {
	unsigned long hwcap = 0;
	cpuinfo_riscv_linux_hwcap_from_getauxval(&hwcap);

	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_I) {
		isa->i = true;
	}
	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_M) {
		isa->m = true;
	}
	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_A) {
		isa->a = true;
	}
	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_F) {
		isa->f = true;
	}
	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_D) {
		isa->d = true;
	}
	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_C) {
		isa->c = true;
	}
	if (hwcap & CPUINFO_RISCV_LINUX_FEATURE_V) {
		isa->v = true;
	}
}
