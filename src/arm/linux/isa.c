#include <stdint.h>

#include <arm/linux/api.h>
#include <arm/linux/cp.h>
#include <log.h>


void cpuinfo_arm_linux_decode_isa_from_proc_cpuinfo(
	const struct proc_cpuinfo proc_cpuinfo[restrict static 1],
	uint32_t proc_cpuinfo_count,
	struct cpuinfo_arm_isa isa[restrict static 1])
{
	const uint32_t cpu_part = proc_cpuinfo->part;
	const uint32_t cpu_implementer = proc_cpuinfo->implementer;
	uint32_t architecture = proc_cpuinfo->architecture.version;
	if (architecture >= 8) {
		/*
		 * ARMv7 code running on ARMv8: IDIV, VFP, NEON are always supported,
		 * but only ARMv8 optional features are reported.
		 */
		isa->armv5e  = true;
		isa->armv6   = true;
		isa->armv6k  = true;
		isa->armv7   = true;
		isa->armv7mp = true;
		isa->thumb  = true;
		isa->thumb2 = true;
		isa->vfpv3 = true;
		isa->d32 = true;
		isa->fp16 = true;
		isa->fma = true;
		isa->neon = true;
	} else {
		/* ARMv7 or lower: use feature flags to detect optional features */

		/*
		 * ARM11 (ARM 1136/1156/1176/11 MPCore) processors can report v7 architecture
		 * even though they support only ARMv6 instruction set.
		 * Detecting this situation by CPU implementer == 'A' (ARM) and CPU part 0xBXX.
		 */
		if (architecture == 7 && cpu_implementer == 'A' && (cpu_part & 0xF00) == 0xB00) {
			cpuinfo_log_warning("Kernel-reported architecture ARMv7 ignored due to mismatch with processor microarchitecture (ARM11)");
			architecture = 6;
		}

		const uint32_t features = proc_cpuinfo->features;
		if ((architecture >= 6) || (features & PROC_CPUINFO_FEATURE_EDSP) || (proc_cpuinfo->architecture.flags & PROC_CPUINFO_ARCH_E)) {
			cpuinfo_isa.armv5e = true;
		}
		if (architecture >= 6) {
			cpuinfo_isa.armv6 = true;
		}
		if (architecture >= 7) {
			cpuinfo_isa.armv6k = true;
			cpuinfo_isa.armv7 = true;

			if (proc_cpuinfo_count > 1) {
				cpuinfo_isa.armv7mp = true;
			}
		}

		if (features & PROC_CPUINFO_FEATURE_IWMMXT) {
			const uint32_t wcid = read_wcid();
			const uint32_t coprocessor_type = (wcid >> 8) & UINT32_C(0xFF);
			if (coprocessor_type >= 0x10) {
				cpuinfo_isa.wmmx = true;
				if (coprocessor_type >= 0x20) {
					cpuinfo_isa.wmmx2 = true;
				}
			} else {
				cpuinfo_log_warning("WMMX ISA disabled: OS reported iwmmxt feature, "
					"but WCID coprocessor type 0x%"PRIx32" indicates no WMMX support",
					coprocessor_type);
			}
		}

		if ((features & PROC_CPUINFO_FEATURE_THUMB) || (proc_cpuinfo->architecture.flags & PROC_CPUINFO_ARCH_T)) {
			cpuinfo_isa.thumb = true;

			/*
			 * There is no separate feature flag for Thumb 2.
			 * All ARMv7 processors and ARM 1156 (CPU part 0xB56) support Thumb 2.
			 */
			if (architecture >= 7 || (cpu_implementer == 'A' && cpu_part == 0xB56)) {
				cpuinfo_isa.thumb2 = true;
			}
		}
		if (features & PROC_CPUINFO_FEATURE_THUMBEE) {
			cpuinfo_isa.thumbee = true;
		}
		if ((features & PROC_CPUINFO_FEATURE_JAVA) || (proc_cpuinfo->architecture.flags & PROC_CPUINFO_ARCH_J)) {
			cpuinfo_isa.jazelle = true;
		}

		if ((features & PROC_CPUINFO_FEATURE_IDIV) == PROC_CPUINFO_FEATURE_IDIV) {
			cpuinfo_isa.idiv = true;
		}
		const uint32_t vfp_mask = \
			PROC_CPUINFO_FEATURE_VFP | PROC_CPUINFO_FEATURE_VFPV3 | PROC_CPUINFO_FEATURE_VFPV3D16 | \
			PROC_CPUINFO_FEATURE_VFPD32 | PROC_CPUINFO_FEATURE_VFPV4 | PROC_CPUINFO_FEATURE_NEON;
		if (features & vfp_mask) {
			const uint32_t vfpv3_mask = PROC_CPUINFO_FEATURE_VFPV3 | PROC_CPUINFO_FEATURE_VFPV3D16 | \
				PROC_CPUINFO_FEATURE_VFPD32 | PROC_CPUINFO_FEATURE_VFPV4 | PROC_CPUINFO_FEATURE_NEON;
			if ((architecture >= 7) | (features & vfpv3_mask)) {
				cpuinfo_isa.vfpv3 = true;
			
				const uint32_t d32_mask = PROC_CPUINFO_FEATURE_VFPD32 | PROC_CPUINFO_FEATURE_NEON;
				if (features & d32_mask) {
					cpuinfo_isa.d32 = true;
				}
			} else {
				const uint32_t fpsid = read_fpsid();
				const uint32_t subarchitecture = (fpsid >> 16) & UINT32_C(0x7F);
				if (subarchitecture >= 0x01) {
					cpuinfo_isa.vfpv2 = true;
				}
			}
		}
		if (features & PROC_CPUINFO_FEATURE_NEON) {
			cpuinfo_isa.neon = true;
		}

		/*
		 * There is no separate feature flag for FP16 support.
		 * VFPv4 implies VFPv3-FP16 support (and in practice, NEON-HP as well).
		 * Additionally, ARM Cortex-A9 (CPU part 0xC09) supports FP16.
		 */
		if ((features & PROC_CPUINFO_FEATURE_VFPV4) || (cpu_implementer == 'A' && cpu_part == 0xC09)) {
			cpuinfo_isa.fp16 = true;
		}

		if (features & PROC_CPUINFO_FEATURE_VFPV4) {
			cpuinfo_isa.fma = true;
		}
	}

	const uint32_t features2 = proc_cpuinfo->features2;
	if (features2 & PROC_CPUINFO_FEATURE2_AES) {
		cpuinfo_isa.aes = true;
	}
	if (features2 & PROC_CPUINFO_FEATURE2_PMULL) {
		cpuinfo_isa.pmull = true;
	}
	if (features2 & PROC_CPUINFO_FEATURE2_SHA1) {
		cpuinfo_isa.sha1 = true;
	}
	if (features2 & PROC_CPUINFO_FEATURE2_SHA2) {
		cpuinfo_isa.sha2 = true;
	}
	if (features2 & PROC_CPUINFO_FEATURE2_CRC32) {
		cpuinfo_isa.crc32 = true;
	}
}
