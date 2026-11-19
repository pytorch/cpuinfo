#include <stdint.h>

#include <cpuinfo/log.h>
#include <riscv/api.h>

void cpuinfo_riscv_decode_vendor_uarch(
	uint32_t vendor_id,
	uint32_t arch_id,
	uint32_t imp_id,
	enum cpuinfo_vendor vendor[restrict static 1],
	enum cpuinfo_uarch uarch[restrict static 1]) {
	*uarch = cpuinfo_uarch_unknown;
	/* The vendor ID is sufficient to determine the cpuinfo_vendor. */
	switch (vendor_id) {
		case cpuinfo_riscv_chipset_vendor_sifive:
			*vendor = cpuinfo_vendor_sifive;
			switch (arch_id & 0x7FFFFFFFU) {
				case 0x00000907:
				case 0x00FFFB07:
					*uarch = cpuinfo_uarch_sifive_x280;
					break;
				case 0x00001007:
				case 0x00FFFC07:
					*uarch = cpuinfo_uarch_sifive_x390;
					break;
				default:
					if (imp_id == 0x07230908) {
						*uarch = cpuinfo_uarch_sifive_x280;
					} else {
						cpuinfo_log_warning(
							"unknown SiFive architecture ID: 0x%08" PRIx32, arch_id);
					}
					break;
			}
			break;
		default:
			*vendor = cpuinfo_vendor_unknown;
			cpuinfo_log_warning("unknown vendor ID: %" PRIu32, vendor_id);
			break;
	}
}
