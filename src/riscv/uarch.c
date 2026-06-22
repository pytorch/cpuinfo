#include <stdint.h>

#include <cpuinfo/log.h>
#include <riscv/api.h>

/*
 * SiFive 7-series marchid values.
 * The high bit (63) indicates a commercial implementation per RISC-V spec.
 * Some revisions of the U74/FU740 report marchid=0x1 instead.
 */
#define SIFIVE_MARCHID_7_SERIES UINT64_C(0x8000000000000007)
#define SIFIVE_MARCHID_7_SERIES_ALT UINT64_C(0x1)

/*
 * T-Head C908 marchid. Earlier T-Head c9xx cores (C906, C910, C920)
 * report marchid=0 and mimpid=0, so they cannot be distinguished
 * individually and are grouped as cpuinfo_uarch_thead_c9xx.
 */
#define THEAD_MARCHID_C908 UINT64_C(0x8d143000)

/* SpacemiT X60 (Veyron V1) marchid. */
#define SPACEMIT_MARCHID_X60 UINT64_C(0x8000000000010000)

void cpuinfo_riscv_decode_vendor_uarch(
	uint64_t vendor_id,
	uint64_t arch_id,
	uint64_t imp_id,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]) {
	switch (vendor_id) {
		case cpuinfo_riscv_chipset_vendor_sifive:
			*vendor = cpuinfo_vendor_sifive;
			switch (arch_id) {
				case SIFIVE_MARCHID_7_SERIES:
				case SIFIVE_MARCHID_7_SERIES_ALT:
					*uarch = cpuinfo_uarch_sifive_7_series;
					break;
				default:
					*uarch = cpuinfo_uarch_unknown;
					break;
			}
			break;
		case cpuinfo_riscv_chipset_vendor_thead:
			*vendor = cpuinfo_vendor_thead;
			switch (arch_id) {
				case THEAD_MARCHID_C908:
					*uarch = cpuinfo_uarch_thead_c908;
					break;
				case 0:
					/*
					 * T-Head c9xx cores (C906, C910, C920) all
					 * report marchid=0. They can only be
					 * differentiated by ISA extensions.
					 */
					*uarch = cpuinfo_uarch_thead_c9xx;
					break;
				default:
					*uarch = cpuinfo_uarch_unknown;
					break;
			}
			break;
		case cpuinfo_riscv_chipset_vendor_spacemit:
			*vendor = cpuinfo_vendor_spacemit;
			switch (arch_id) {
				case SPACEMIT_MARCHID_X60:
					*uarch = cpuinfo_uarch_spacemit_x60;
					break;
				default:
					*uarch = cpuinfo_uarch_unknown;
					break;
			}
			break;
		default:
			*vendor = cpuinfo_vendor_unknown;
			*uarch = cpuinfo_uarch_unknown;
			if (vendor_id != 0) {
				cpuinfo_log_warning("unknown vendor ID: %" PRIu64, vendor_id);
			}
			break;
	}
}
