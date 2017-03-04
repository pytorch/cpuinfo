#include <stdint.h>
#include <stdlib.h>

#include <cpuinfo.h>
#include <x86/api.h>
#include <mach/api.h>
#include <api.h>
#include <log.h>


static inline uint32_t max(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}

void cpuinfo_x86_mach_init(void) {
	struct cpuinfo_mach_topology mach_topology = cpuinfo_mach_detect_topology();
	struct cpuinfo_processor* processors = calloc(mach_topology.threads, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
			mach_topology.threads * sizeof(struct cpuinfo_processor), mach_topology.threads);
		return;
	}

	struct cpuinfo_x86_processor x86_processor;
	cpuinfo_x86_init_processor(&x86_processor);

	const uint32_t threads_per_core = mach_topology.threads / mach_topology.cores;
	const uint32_t threads_per_package = mach_topology.threads / mach_topology.packages;
	for (uint32_t t = 0; t < mach_topology.threads; t++) {
		processors[t].vendor = x86_processor.vendor;
		processors[t].uarch = x86_processor.uarch;

		/* Reconstruct APIC IDs from topology components */
		const uint32_t thread_bits_mask = (UINT32_C(1) << x86_processor.topology.thread_bits_length) - UINT32_C(1);
		const uint32_t core_bits_mask   = (UINT32_C(1) << x86_processor.topology.core_bits_length)   - UINT32_C(1);

		const uint32_t smt_id = t % threads_per_core;
		const uint32_t core_id = t / threads_per_core;
		const uint32_t package_id = t / threads_per_package;
		const uint32_t package_bits_offset = max(
			x86_processor.topology.smt_bits_offset + x86_processor.topology.smt_bits_length,
			x86_processor.topology.core_bits_offset + x86_processor.topology.core_bits_length);

		const uint32_t apic_id =
			((smt_id & smt_bits_mask) << x86_processor.topology.smt_bits_offset) |
			((core_id & core_bits_mask) << x86_processor.topology.core_bits_offset) |
			(package_id << package_bits_offset);
		processors[t].topology = (cpuinfo_topology) {
			.thread_id  = smt_id,
			.core_id    = core_id,
			.package_id = package_id
		};
		cpuinfo_log_info("reconstructed APIC ID 0x%08"PRIx32" for thread %"PRIu32, apic_id, t);
	}

	uint32_t threads_per_l1 = 0, l1_count = 0;
	if (x86_processor.cache.l1i.size != 0 || x86_processor.cache.l1d.size != 0) {
		threads_per_l1 = mach_topology.threads_per_cache[1];
		if (threads_per_l1 == 0) {
			/* Assume that threads on the same core share L1 */
			threads_per_l1 = mach_topology.threads / mach_topology.cores;
			cpuinfo_log_warning("Mach kernel did not report number of threads sharing L1 cache; assume %"PRIu32,
				threads_per_l1);
		}
		l1_count = mach_topology.threads / threads_per_l1;
		cpuinfo_log_debug("detected %"PRIu32" L1 caches", l1_count);
	}

	uint32_t threads_per_l2 = 0, l2_count = 0;
	if (x86_processor.cache.l2.size != 0) {
		threads_per_l2 = mach_topology.threads_per_cache[2];
		if (threads_per_l2 == 0) {
			if (x86_processor.cache.l3.size != 0) {
				/* This is not a last-level cache; assume that threads on the same core share L2 */
				threads_per_l2 = mach_topology.threads / mach_topology.cores;
			} else {
				/* This is a last-level cache; assume that threads on the same package share L2 */
				threads_per_l2 = mach_topology.threads / mach_topology.packages;
			}
			cpuinfo_log_warning("Mach kernel did not report number of threads sharing L2 cache; assume %"PRIu32,
				threads_per_l2);
		}
		l2_count = mach_topology.threads / threads_per_l2;
		cpuinfo_log_debug("detected %"PRIu32" L2 caches", l2_count);
	}

	uint32_t threads_per_l3 = 0, l3_count = 0;
	if (x86_processor.cache.l3.size != 0) {
		threads_per_l3 = mach_topology.threads_per_cache[3];
		if (threads_per_l3 == 0) {
			/*
			 * Assume that threads on the same package share L3.
			 * However, is it not necessarily the last-level cache (there may be L4 cache as well)
			 */
			threads_per_l3 = mach_topology.threads / mach_topology.packages;
			cpuinfo_log_warning("Mach kernel did not report number of threads sharing L3 cache; assume %"PRIu32,
				threads_per_l3);
		}
		l3_count = mach_topology.threads / threads_per_l3;
		cpuinfo_log_debug("detected %"PRIu32" L3 caches", l3_count);
	}

	uint32_t threads_per_l4 = 0, l4_count = 0;
	if (x86_processor.cache.l4.size != 0) {
		threads_per_l4 = mach_topology.threads_per_cache[4];
		if (threads_per_l4 == 0) {
			/*
			 * Assume that all threads share this L4.
			 * As of now, L4 cache exists only on notebook x86 CPUs, which are single-package,
			 * but multi-socket systems could have shared L4 (like on IBM POWER8).
			 */
			threads_per_l4 = mach_topology.threads;
			cpuinfo_log_warning("Mach kernel did not report number of threads sharing L4 cache; assume %"PRIu32,
				threads_per_l4);
		}
		l4_count = mach_topology.threads / threads_per_l4;
		cpuinfo_log_debug("detected %"PRIu32" L4 caches", l4_count);
	}

	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	struct cpuinfo_cache* l3 = NULL;
	struct cpuinfo_cache* l4 = NULL;

	if (x86_processor.cache.l1i.size != 0) {
		l1i = calloc(l1_count, sizeof(struct cpuinfo_cache));
		if (l1i == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
				l1_count * sizeof(struct cpuinfo_cache), l1_count);
			return;
		}
		for (uint32_t c = 0; c < l1_count; c++) {
			l1i[c] = (struct cpuinfo_cache) {
				.size          = x86_processor.cache.l1i.size,
				.associativity = x86_processor.cache.l1i.associativity,
				.sets          = x86_processor.cache.l1i.sets,
				.partitions    = x86_processor.cache.l1i.partitions,
				.line_size     = x86_processor.cache.l1i.line_size,
				.flags         = x86_processor.cache.l1i.flags,
				.thread_start  = c * threads_per_l1,
				.thread_count  = threads_per_l1
			};
		}
		for (uint32_t t = 0; t < mach_topology.threads; t++) {
			processors[t].cache.l1i = &l1i[t / threads_per_l1];
		}
	}

	if (x86_processor.cache.l1d.size != 0) {
		l1d = calloc(l1_count, sizeof(struct cpuinfo_cache));
		if (l1d == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
				l1_count * sizeof(struct cpuinfo_cache), l1_count);
			return;
		}
		for (uint32_t c = 0; c < l1_count; c++) {
			l1d[c] = (struct cpuinfo_cache) {
				.size          = x86_processor.cache.l1d.size,
				.associativity = x86_processor.cache.l1d.associativity,
				.sets          = x86_processor.cache.l1d.sets,
				.partitions    = x86_processor.cache.l1d.partitions,
				.line_size     = x86_processor.cache.l1d.line_size,
				.flags         = x86_processor.cache.l1d.flags,
				.thread_start  = c * threads_per_l1,
				.thread_count  = threads_per_l1
			};
		}
		for (uint32_t t = 0; t < mach_topology.threads; t++) {
			processors[t].cache.l1d = &l1d[t / threads_per_l1];
		}
	}

	if (l2_count != 0) {
		l2 = calloc(l2_count, sizeof(struct cpuinfo_cache));
		if (l1d == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2_count * sizeof(struct cpuinfo_cache), l2_count);
			return;
		}
		for (uint32_t c = 0; c < l2_count; c++) {
			l2[c] = (struct cpuinfo_cache) {
				.size          = x86_processor.cache.l2.size,
				.associativity = x86_processor.cache.l2.associativity,
				.sets          = x86_processor.cache.l2.sets,
				.partitions    = x86_processor.cache.l2.partitions,
				.line_size     = x86_processor.cache.l2.line_size,
				.flags         = x86_processor.cache.l2.flags,
				.thread_start  = c * threads_per_l2,
				.thread_count  = threads_per_l2
			};
		}
		for (uint32_t t = 0; t < mach_topology.threads; t++) {
			processors[t].cache.l2 = &l2[t / threads_per_l1];
		}
	}

	if (l3_count != 0) {
		l3 = calloc(l3_count, sizeof(struct cpuinfo_cache));
		if (l3 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L3 caches",
				l3_count * sizeof(struct cpuinfo_cache), l3_count);
			return;
		}
		for (uint32_t c = 0; c < l3_count; c++) {
			l3[c] = (struct cpuinfo_cache) {
				.size          = x86_processor.cache.l3.size,
				.associativity = x86_processor.cache.l3.associativity,
				.sets          = x86_processor.cache.l3.sets,
				.partitions    = x86_processor.cache.l3.partitions,
				.line_size     = x86_processor.cache.l3.line_size,
				.flags         = x86_processor.cache.l3.flags,
				.thread_start  = c * threads_per_l3,
				.thread_count  = threads_per_l3
			};
		}
		for (uint32_t t = 0; t < mach_topology.threads; t++) {
			processors[t].cache.l3 = &l3[t / threads_per_l1];
		}
	}

	if (l4_count != 0) {
		l4 = calloc(l4_count, sizeof(struct cpuinfo_cache));
		if (l4 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L4 caches",
				l4_count * sizeof(struct cpuinfo_cache), l4_count);
			return;
		}
		for (uint32_t c = 0; c < l4_count; c++) {
			l4[c] = (struct cpuinfo_cache) {
				.size          = x86_processor.cache.l4.size,
				.associativity = x86_processor.cache.l4.associativity,
				.sets          = x86_processor.cache.l4.sets,
				.partitions    = x86_processor.cache.l4.partitions,
				.line_size     = x86_processor.cache.l4.line_size,
				.flags         = x86_processor.cache.l4.flags,
				.thread_start  = c * threads_per_l4,
				.thread_count  = threads_per_l4
			};
		}
		for (uint32_t t = 0; t < mach_topology.threads; t++) {
			processors[t].cache.l4 = &l4[t / threads_per_l1];
		}
	}

	/* Commit changes */
	cpuinfo_processors = processors;
	cpuinfo_l1i_cache = l1i;
	cpuinfo_l1d_cache = l1d;
	cpuinfo_l2_cache = l2;
	cpuinfo_l3_cache = l3;
	cpuinfo_l4_cache = l4;
}
