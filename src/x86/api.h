#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>


struct cpuid_regs {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
};

struct cpuinfo_x86_cache {
	uint32_t size;
	uint32_t associativity;
	uint32_t sets;
	uint32_t partitions;
	uint32_t line_size;
	uint32_t flags;
	uint32_t apic_bits;
};

struct cpuinfo_x86_caches {
	struct cpuinfo_trace_cache trace;
	struct cpuinfo_x86_cache l1i;
	struct cpuinfo_x86_cache l1d;
	struct cpuinfo_x86_cache l2;
	struct cpuinfo_x86_cache l3;
	struct cpuinfo_x86_cache l4;
	uint32_t prefetch_size;
};

struct cpuinfo_x86_topology {
	#ifdef __linux__
		int linux_id;
	#endif
	uint32_t apic_id;
	uint32_t thread_bits_offset;
	uint32_t thread_bits_length;
	uint32_t core_bits_offset;
	uint32_t core_bits_length;
};

struct cpuinfo_x86_processor {
	enum cpuinfo_vendor vendor;
	enum cpuinfo_uarch uarch;
	struct cpuinfo_x86_model_info model_info;
	struct cpuinfo_x86_caches cache;
	struct {
		struct cpuinfo_tlb itlb_4KB;
		struct cpuinfo_tlb itlb_2MB;
		struct cpuinfo_tlb itlb_4MB;
		struct cpuinfo_tlb dtlb0_4KB;
		struct cpuinfo_tlb dtlb0_2MB;
		struct cpuinfo_tlb dtlb0_4MB;
		struct cpuinfo_tlb dtlb_4KB;
		struct cpuinfo_tlb dtlb_2MB;
		struct cpuinfo_tlb dtlb_4MB;
		struct cpuinfo_tlb dtlb_1GB;
		struct cpuinfo_tlb stlb2_4KB;
		struct cpuinfo_tlb stlb2_2MB;
		struct cpuinfo_tlb stlb2_1GB;
	} tlb;
	struct cpuinfo_x86_topology topology;
};

void cpuinfo_x86_init_processor(struct cpuinfo_x86_processor processor[restrict static 1]);

enum cpuinfo_vendor cpuinfo_x86_decode_vendor(uint32_t ebx, uint32_t ecx, uint32_t edx);
struct cpuinfo_x86_model_info cpuinfo_x86_decode_model_info(uint32_t eax);
enum cpuinfo_uarch cpuinfo_x86_decode_uarch(
	enum cpuinfo_vendor vendor,
	const struct cpuinfo_x86_model_info model_info[restrict static 1]);

struct cpuinfo_x86_isa cpuinfo_x86_detect_isa(const struct cpuid_regs basic_info,
	uint32_t max_base_index, uint32_t max_extended_index, enum cpuinfo_vendor vendor);
struct cpuinfo_x86_isa cpuinfo_x86_nacl_detect_isa(void);

void cpuinfo_x86_detect_topology(
	uint32_t max_base_index,
	uint32_t max_extended_index,
	struct cpuid_regs leaf1,
	struct cpuinfo_x86_topology topology[restrict static 1]);

void cpuinfo_x86_detect_cache(
	uint32_t max_base_index, uint32_t max_extended_index,
	enum cpuinfo_vendor vendor,
	const struct cpuinfo_x86_model_info model_info[restrict static 1],
	struct cpuinfo_x86_caches cache[restrict static 1],
	struct cpuinfo_tlb itlb_4KB[restrict static 1],
	struct cpuinfo_tlb itlb_2MB[restrict static 1],
	struct cpuinfo_tlb itlb_4MB[restrict static 1],
	struct cpuinfo_tlb dtlb0_4KB[restrict static 1],
	struct cpuinfo_tlb dtlb0_2MB[restrict static 1],
	struct cpuinfo_tlb dtlb0_4MB[restrict static 1],
	struct cpuinfo_tlb dtlb_4KB[restrict static 1],
	struct cpuinfo_tlb dtlb_2MB[restrict static 1],
	struct cpuinfo_tlb dtlb_4MB[restrict static 1],
	struct cpuinfo_tlb dtlb_1GB[restrict static 1],
	struct cpuinfo_tlb stlb2_4KB[restrict static 1],
	struct cpuinfo_tlb stlb2_2MB[restrict static 1],
	struct cpuinfo_tlb stlb2_1GB[restrict static 1],
	uint32_t log2_package_cores_max[restrict static 1]);

void cpuinfo_x86_decode_cache_descriptor(
	uint8_t descriptor, enum cpuinfo_vendor vendor,
	const struct cpuinfo_x86_model_info model_info[restrict static 1],
	struct cpuinfo_x86_caches cache[restrict static 1],
	struct cpuinfo_tlb itlb_4KB[restrict static 1],
	struct cpuinfo_tlb itlb_2MB[restrict static 1],
	struct cpuinfo_tlb itlb_4MB[restrict static 1],
	struct cpuinfo_tlb dtlb0_4KB[restrict static 1],
	struct cpuinfo_tlb dtlb0_2MB[restrict static 1],
	struct cpuinfo_tlb dtlb0_4MB[restrict static 1],
	struct cpuinfo_tlb dtlb_4KB[restrict static 1],
	struct cpuinfo_tlb dtlb_2MB[restrict static 1],
	struct cpuinfo_tlb dtlb_4MB[restrict static 1],
	struct cpuinfo_tlb dtlb_1GB[restrict static 1],
	struct cpuinfo_tlb stlb2_4KB[restrict static 1],
	struct cpuinfo_tlb stlb2_2MB[restrict static 1],
	struct cpuinfo_tlb stlb2_1GB[restrict static 1],
	uint32_t prefetch_size[restrict static 1]);

bool cpuinfo_x86_decode_deterministic_cache_parameters(
	struct cpuid_regs regs,
	struct cpuinfo_x86_caches cache[restrict static 1],
	uint32_t package_cores_max[restrict static 1]);
