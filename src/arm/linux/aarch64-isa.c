#include <stdint.h>

#include <arm/linux/api.h>
#include <cpuinfo/log.h>


void cpuinfo_arm64_linux_decode_isa_from_proc_cpuinfo(
	uint32_t features,
	uint32_t midr,
	const struct cpuinfo_arm_chipset chipset[restrict static 1],
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
		if (chipset->series == cpuinfo_arm_chipset_series_samsung_exynos && chipset->model == 9810) {
			/* Exynos 9810 reports that it supports FP16 compute, but in fact only little cores do */
			cpuinfo_log_warning("FP16 arithmetics disabled: only little cores of Exynos 9810 support FP16 compute");
		} else {
			isa->fp16arith = true;
		}
	} else if (features & CPUINFO_ARM_LINUX_FEATURE_FPHP) {
		cpuinfo_log_warning("FP16 arithmetics disabled: detected support only for scalar operations");
	} else if (features & CPUINFO_ARM_LINUX_FEATURE_ASIMDHP) {
		cpuinfo_log_warning("FP16 arithmetics disabled: detected support only for SIMD operations");
	}
	/*
	 * Many phones ship with an old kernel configuration that doesn't report
	 * SQRDMLAH/SQRDMLSH/UQRDMLAH/UQRDMLSH instructions.
	 * Use a MIDR-based heuristic to whitelist processors known to support it:
	 * - Processors with Qualcomm-modified Cortex-A55 cores
	 * - Processors with Qualcomm-modified Cortex-A75 cores
	 * - Processors with Qualcomm-modified Cortex-A76 cores
	 * - Kirin 980 processor
	 */
	switch (midr & (CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK)) {
		case UINT32_C(0x51008020): /* Kryo 385 Gold (Cortex-A75) */
		case UINT32_C(0x51008030): /* Kryo 385 Silver (Cortex-A55) */
		case UINT32_C(0x51008040): /* Kryo 485 Gold (Cortex-A76) */
			isa->rdm = true;
			break;
		default:
			if (features & CPUINFO_ARM_LINUX_FEATURE_ASIMDRDM) {
				isa->rdm = true;
			}
			if (chipset->series == cpuinfo_arm_chipset_series_hisilicon_kirin && chipset->model == 980) {
				isa->rdm = true;
			}
			break;
	}
	/*
	 * Many phones ship with an old kernel configuration that doesn't report UDOT/SDOT instructions.
	 * Use a MIDR-based heuristic to whitelist processors known to support it:
	 * - Processors with Qualcomm-modified Cortex-A76 cores
	 * - Kirin 980 processor
	 */
	switch (midr & (CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK)) {
		case UINT32_C(0x51008040): /* Kryo 485 Gold (Cortex-A76) */
			isa->dot = true;
			break;
		default:
			if (features & CPUINFO_ARM_LINUX_FEATURE_ASIMDDP) {
				isa->dot = true;
			}
			if (chipset->series == cpuinfo_arm_chipset_series_hisilicon_kirin && chipset->model == 980) {
				isa->dot = true;
			}
			break;
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
