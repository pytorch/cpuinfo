#include <stdint.h>

#include <arm/api.h>
#include <log.h>


void cpuinfo_arm_decode_vendor_uarch(
	uint32_t cpu_implementer,
	uint32_t cpu_part,
#if CPUINFO_ARCH_ARM
	bool has_vfpv4,
#endif /* CPUINFO_ARCH_ARM */
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1])
{
	switch (cpu_implementer) {
		case 'A':
			*vendor = cpuinfo_vendor_arm;
			switch (cpu_part) {
#if CPUINFO_ARCH_ARM
				case 0xC05:
					*uarch = cpuinfo_uarch_cortex_a5;
					break;
				case 0xC07:
					*uarch = cpuinfo_uarch_cortex_a7;
					break;
				case 0xC08:
					*uarch = cpuinfo_uarch_cortex_a8;
					break;
				case 0xC09:
					*uarch = cpuinfo_uarch_cortex_a9;
					break;
				case 0xC0C:
					*uarch = cpuinfo_uarch_cortex_a12;
					break;
				case 0xC0E:
					*uarch = cpuinfo_uarch_cortex_a17;
					break;
				case 0xC0F:
					*uarch = cpuinfo_uarch_cortex_a15;
					break;
#endif /* CPUINFO_ARCH_ARM */
				case 0xD01:
					*uarch = cpuinfo_uarch_cortex_a32;
					break;
				case 0xD03:
					*uarch = cpuinfo_uarch_cortex_a53;
					break;
				case 0xD04:
					*uarch = cpuinfo_uarch_cortex_a35;
					break;
				case 0xD07:
					*uarch = cpuinfo_uarch_cortex_a57;
					break;
				case 0xD08:
					*uarch = cpuinfo_uarch_cortex_a72;
					break;
				case 0xD09:
					*uarch = cpuinfo_uarch_cortex_a73;
					break;
				default:
					switch (cpu_part >> 8) {
						case 7:
							*uarch = cpuinfo_uarch_arm7;
							break;
						case 9:
							*uarch = cpuinfo_uarch_arm9;
							break;
						case 11:
							*uarch = cpuinfo_uarch_arm11;
							break;
						default:
							cpuinfo_log_warning("unknown ARM CPU part 0x%03"PRIx32" ignored", cpu_part);
					}
			}
			break;
		case 'C':
			*vendor = cpuinfo_vendor_cavium;
			switch (cpu_part) {
				case 0x0A1:
					*uarch = cpuinfo_uarch_thunderx;
					break;
				default:
					cpuinfo_log_warning("unknown Cavium CPU part 0x%03"PRIx32" ignored", cpu_part);
			}
			break;
#if CPUINFO_ARCH_ARM
		case 'i':
			*vendor = cpuinfo_vendor_intel;
			switch (cpu_part >> 8) {
				case 2: /* PXA 210/25X/26X */
				case 4: /* PXA 27X */
				case 6: /* PXA 3XX */
					*uarch = cpuinfo_uarch_xscale;
					break;
				default:
					cpuinfo_log_warning("unknown Intel CPU part 0x%03"PRIx32" ignored", cpu_part);
			}
			break;
#endif /* CPUINFO_ARCH_ARM */
		case 'N':
			*vendor = cpuinfo_vendor_nvidia;
			switch (cpu_part) {
				case 0x000:
					*uarch = cpuinfo_uarch_denver;
					break;
				case 0x003:
					/*
					 * nVidia Denver 2.
					 * Few details are known about Denver 2, and known details are no different that Denver 1,
					 * so consider them the same microarchitecture.
					 */
					*uarch = cpuinfo_uarch_denver;
					break;
				default:
					cpuinfo_log_warning("unknown nVidia CPU part 0x%03"PRIx32" ignored", cpu_part);
			}
			break;
		case 'Q':
			*vendor = cpuinfo_vendor_qualcomm;
			switch (cpu_part) {
#if CPUINFO_ARCH_ARM
				case 0x00F:
					/* Mostly Scorpions, but some Cortex A5 may report this value as well */
					if (has_vfpv4) {
						/* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
						*vendor = cpuinfo_vendor_arm;
						*uarch = cpuinfo_uarch_cortex_a5;
					} else {
						*uarch = cpuinfo_uarch_scorpion;
					}
					break;
				case 0x02D: /* Dual-core Scorpions */
					*uarch = cpuinfo_uarch_scorpion;
					break;
				case 0x04D: /* Dual-core Krait */
				case 0x06F: /* Quad-core Krait */
					*uarch = cpuinfo_uarch_krait;
					break;
#endif /* CPUINFO_ARCH_ARM */
				case 0x205: /* Low-power Kryo "Silver" */
				case 0x211: /* High-performance Kryo "Gold" */
					*uarch = cpuinfo_uarch_kryo;
					break;
				case 0x800: /* Low-power Kryo 280 -> Cortex-A53 */
					*vendor = cpuinfo_vendor_arm;
					*uarch = cpuinfo_uarch_cortex_a53;
					break;
				case 0x801: /* High-performance Kryo 280 -> Cortex-A73 */
					*vendor = cpuinfo_vendor_arm;
					*uarch = cpuinfo_uarch_cortex_a73;
					break;
				default:
					cpuinfo_log_warning("unknown Qualcomm CPU part 0x%03"PRIx32" ignored", cpu_part);
			}
			break;
		case 'S':
			*vendor = cpuinfo_vendor_samsung;
			switch (cpu_part) {
				case 0x001:
					*uarch = cpuinfo_uarch_mongoose;
					break;
				default:
					cpuinfo_log_warning("unknown Samsung CPU part 0x%03"PRIx32" ignored", cpu_part);
			}
			break;
#if CPUINFO_ARCH_ARM
		case 'V':
			*vendor = cpuinfo_vendor_marvell;
			cpuinfo_log_warning("unknown Marvell CPU part 0x%03"PRIx32" ignored", cpu_part);
			break;
#endif /* CPUINFO_ARCH_ARM */
		default:
			cpuinfo_log_warning("unknown CPU implementer '%c' (0x%02"PRIx32") with CPU part 0x%03"PRIx32" ignored",
				(char) cpu_implementer, cpu_implementer, cpu_part);
	}
}
