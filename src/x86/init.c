#include <stdint.h>
#include <string.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <cpuinfo/utils.h>
#include <x86/api.h>
#include <x86/cpuid.h>

struct cpuinfo_x86_isa cpuinfo_isa = {0};
CPUINFO_INTERNAL uint32_t cpuinfo_x86_clflush_size = 0;

/*
 * Hybrid (P-core + E-core) state captured during initialization and used by
 * cpuinfo_x86_get_current_uarch_index to resolve the calling core's
 * microarchitecture from CPUID leaf 0x1A without any OS calls.
 */
static bool hybrid = false;
static enum cpuinfo_uarch p_uarch = cpuinfo_uarch_unknown;
static enum cpuinfo_uarch e_uarch = cpuinfo_uarch_unknown;

void cpuinfo_x86_init_processor(struct cpuinfo_x86_processor* processor) {
	const struct cpuid_regs leaf0 = cpuid(0);
	const uint32_t max_base_index = leaf0.eax;
	const enum cpuinfo_vendor vendor = processor->vendor =
		cpuinfo_x86_decode_vendor(leaf0.ebx, leaf0.ecx, leaf0.edx);

	const struct cpuid_regs leaf0x80000000 = cpuid(UINT32_C(0x80000000));
	const uint32_t max_extended_index = leaf0x80000000.eax >= UINT32_C(0x80000000) ? leaf0x80000000.eax : 0;

	const struct cpuid_regs leaf0x80000001 = max_extended_index >= UINT32_C(0x80000001)
		? cpuid(UINT32_C(0x80000001))
		: (struct cpuid_regs){0, 0, 0, 0};

	if (max_base_index >= 1) {
		const struct cpuid_regs leaf1 = cpuid(1);
		processor->cpuid = leaf1.eax;

		const struct cpuinfo_x86_model_info model_info = cpuinfo_x86_decode_model_info(leaf1.eax);
		/* Hybrid requires CPUID leaf 0x1A to tell P-cores from E-cores. */
		const bool is_hybrid =
			max_base_index >= UINT32_C(0x1A) && (cpuidex(7, 0).edx & UINT32_C(0x00008000)) != 0;
		enum cpuinfo_uarch uarch;
		if (is_hybrid) {
			uarch = processor->uarch = cpuinfo_x86_decode_uarch(vendor, &model_info, 0x40);
			processor->e_uarch = cpuinfo_x86_decode_uarch(vendor, &model_info, 0x20);
		} else {
			const uint32_t core_type =
				max_base_index >= UINT32_C(0x1A) ? (cpuidex(UINT32_C(0x1A), 0).eax >> 24) : 0;
			uarch = processor->uarch = processor->e_uarch =
				cpuinfo_x86_decode_uarch(vendor, &model_info, core_type);
		}
		hybrid = is_hybrid;
		p_uarch = processor->uarch;
		e_uarch = processor->e_uarch;

		cpuinfo_x86_clflush_size = ((leaf1.ebx >> 8) & UINT32_C(0x000000FF)) * 8;

		/*
		 * Topology extensions support:
		 * - AMD: ecx[bit 22] in extended info (reserved bit on Intel
		 * CPUs).
		 */
		const bool amd_topology_extensions = !!(leaf0x80000001.ecx & UINT32_C(0x00400000));

		cpuinfo_x86_detect_cache(
			max_base_index,
			max_extended_index,
			amd_topology_extensions,
			vendor,
			&model_info,
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

		cpuinfo_isa = cpuinfo_x86_detect_isa(
			leaf1, leaf0x80000001, max_base_index, max_extended_index, vendor, uarch);
	}
	if (max_extended_index >= UINT32_C(0x80000004)) {
		struct cpuid_regs brand_string[3];
		for (uint32_t i = 0; i < 3; i++) {
			brand_string[i] = cpuid(UINT32_C(0x80000002) + i);
		}
		memcpy(processor->brand_string, brand_string, sizeof(brand_string));
		cpuinfo_log_debug("raw CPUID brand string: \"%48s\"", processor->brand_string);
	}
}

uint32_t cpuinfo_x86_get_current_uarch_index(uint32_t default_uarch_index) {
	if (!hybrid || cpuinfo_uarchs == NULL || cpuinfo_uarchs_count <= 1) {
		/* Special case: avoid CPUID on systems with only a single type of cores */
		return 0;
	}

	/*
	 * CPUID leaf 0x1A EAX[31:24] reports the type of the core executing the
	 * instruction: 0x20 = Intel Atom (E-core), 0x40 = Intel Core (P-core).
	 * Unlike the OS-provided processor number, this needs no system call and
	 * works in restricted environments (e.g. sandboxes) on any OS.
	 */
	const uint32_t core_type = cpuidex(UINT32_C(0x1A), 0).eax >> 24;
	const enum cpuinfo_uarch uarch = core_type == UINT32_C(0x20) ? e_uarch : p_uarch;
	for (uint32_t i = 0; i < cpuinfo_uarchs_count; i++) {
		if (cpuinfo_uarchs[i].uarch == uarch) {
			return i;
		}
	}
	return default_uarch_index;
}
