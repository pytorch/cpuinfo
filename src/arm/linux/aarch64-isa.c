#include <stdint.h>

#include <arm/linux/api.h>
#include <log.h>


void cpuinfo_arm64_linux_decode_isa_from_proc_cpuinfo(
	uint32_t features,
	struct cpuinfo_arm_isa isa[restrict static 1])
{
	if (features & CPUINFO_ARM_LINUX_FEATURE_AES) {
		isa->aes = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_PMULL) {
		isa->pmull = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_SHA1) {
		isa->sha1 = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_SHA2) {
		isa->sha2 = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_CRC32) {
		isa->crc32 = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_ATOMICS) {
		isa->atomics = true;
	}
	const uint32_t fp16arith_mask = CPUINFO_ARM_LINUX_FEATURE_FPHP | CPUINFO_ARM_LINUX_FEATURE_ASIMDHP;
	if ((features & fp16arith_mask) == fp16arith_mask) {
		isa->fp16arith = true;
	} else if (features & CPUINFO_ARM_LINUX_FEATURE_FPHP) {
		cpuinfo_log_warning("FP16 arithmetics disabled: detected support only for scalar operations");
	} else if (features & CPUINFO_ARM_LINUX_FEATURE_ASIMDHP) {
		cpuinfo_log_warning("FP16 arithmetics disabled: detected support only for SIMD operations");
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_ASIMDRDM) {
		isa->rdm = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_JSCVT) {
		isa->jscvt = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_ASIMDRDM) {
		isa->rdm = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_JSCVT) {
		isa->jscvt = true;
	}
	if (features & CPUINFO_ARM_LINUX_FEATURE_FCMA) {
		isa->fcma = true;
	}
}
