#include <inttypes.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

bool cpuinfo_riscv_decode_cache(
        enum cpuinfo_uarch uarch,
        struct cpuinfo_cache l1i[restrict static 1],
        struct cpuinfo_cache l1d[restrict static 1],
        struct cpuinfo_cache l2[restrict static 1]) {
    switch(uarch) {
		// According to https://starfivetech.com/uploads/u74mc_core_complex_manual_21G1.pdf
        case cpuinfo_uarch_u74_mc:
           *l1i = (struct cpuinfo_cache) {
               .size = 32 * 1024,
               .associativity = 2,
               .line_size = 64
           };
           *l1d = (struct cpuinfo_cache) {
               .size = 32 * 1024,
               .associativity = 4,
               .line_size = 64
           };
           *l2 = (struct cpuinfo_cache) {
                .size = 2 * 1024 * 1024,
                .associativity = 16,
                .line_size = 64
           };
           break;
        default:
            cpuinfo_log_warning("target uarch not recognized; cache data is not populated");
			return false;
    }
    l1i->sets = l1i->size / (l1i->associativity * l1i->line_size);
    l1i->partitions = 1;
    l1d->sets = l1d->size / (l1d->associativity * l1d->line_size);
    l1d->partitions = 1;
    if (l2->size != 0) {
        l2->sets = l2->size / (l2->associativity * l2->line_size);
        l2->partitions = 1;
    }

	return true;
}

uint32_t cpuinfo_riscv_compute_max_cache_size(const struct cpuinfo_processor* processor) {
    switch(processor->core->uarch) {
		// According to https://starfivetech.com/uploads/u74mc_core_complex_manual_21G1.pdf
        case cpuinfo_uarch_u74_mc:
            return 2 * 1024 * 1024;
        default:
            cpuinfo_log_warning("target uarch not recognized; max cache size is not populated");
            return 0;
    }
}