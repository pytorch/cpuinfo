#include <stdint.h>

#include <loongarch/linux/api.h>
#include <cpuinfo/log.h>


void cpuinfo_loongarch64_linux_decode_isa_from_proc_cpuinfo(
	uint32_t features,
	struct cpuinfo_loongarch_isa isa[restrict static 1])
{
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_CPUCFG) {
		isa->cpucfg = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LAM) {
		isa->lam = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_UAL) {
		isa->ual = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_FPU) {
		isa->fpu = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LSX) {
		isa->lsx = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LASX) {
		isa->lasx = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_CRC32) {
		isa->crc32 = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_COMPLEX) {
		isa->complex = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_CRYPTO) {
		isa->crypto = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LVZ) {
		isa->lvz = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_X86) {
		isa->lbt_x86 = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_ARM) {
		isa->lbt_arm = true;
	}
	if (features & CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_MIPS) {
		isa->lbt_mips = true;
	}
}
