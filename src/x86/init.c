#include <stdint.h>

#include <cpuinfo.h>
#include <x86/cpuid.h>
#include <x86/api.h>
#include <log.h>


struct cpuinfo_x86_isa cpuinfo_isa = { 0 };
uint32_t cpuinfo_x86_clflush_size = 0;

void cpuinfo_x86_init_processor(struct cpuinfo_x86_processor processor[restrict static 1]) {
	const struct cpuid_regs leaf0 = cpuid(0);
	const uint32_t max_base_index = leaf0.eax;
	const enum cpuinfo_vendor vendor = processor->vendor =
		cpuinfo_x86_decode_vendor(leaf0.ebx, leaf0.ecx, leaf0.edx);

	const struct cpuid_regs leaf0x80000000 = cpuid(UINT32_C(0x80000000));
	const uint32_t max_extended_index =
		leaf0x80000000.eax >= UINT32_C(0x80000000) ? leaf0x80000000.eax : 0;

	if (max_base_index >= 1) {
		const struct cpuid_regs leaf1 = cpuid(1);
		const struct cpuinfo_x86_model_info model_info = cpuinfo_x86_decode_model_info(leaf1.eax);
		const enum cpuinfo_uarch uarch = processor->uarch =
			cpuinfo_x86_decode_uarch(vendor, &model_info);

		const uint32_t brand_index = leaf1.ebx & UINT32_C(0x000000FF);
		cpuinfo_x86_clflush_size = ((leaf1.ebx >> 8) & UINT32_C(0x000000FF)) * 8;

		cpuinfo_x86_detect_cache(max_base_index, max_extended_index, vendor, &model_info,
			&processor->cache,
			&processor->tlb.itlb_4KB,
			&processor->tlb.itlb_2MB,
			&processor->tlb.itlb_4MB,
			&processor->tlb.dtlb0_4KB,
			&processor->tlb.dtlb0_2MB,
			&processor->tlb.dtlb0_4MB,
			&processor->tlb.dtlb_4KB,
			&processor->tlb.dtlb_2MB,
			&processor->tlb.dtlb_4MB,
			&processor->tlb.dtlb_1GB,
			&processor->tlb.stlb2_4KB,
			&processor->tlb.stlb2_2MB,
			&processor->tlb.stlb2_1GB,
			&processor->topology.core_bits_length);

		cpuinfo_x86_detect_topology(max_base_index, max_extended_index, leaf1, &processor->topology);

		#ifdef __native_client__
			cpuinfo_isa = cpuinfo_x86_nacl_detect_isa();			
		#else
			cpuinfo_isa = cpuinfo_x86_detect_isa(leaf1, max_base_index, max_extended_index, vendor, uarch);
		#endif
	}
}
