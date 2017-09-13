#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sched.h>

#include <cpuinfo.h>
#include <x86/api.h>
#include <linux/api.h>
#include <api.h>
#include <log.h>


static inline uint32_t max(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}

static inline uint32_t bit_mask(uint32_t bits) {
	return (UINT32_C(1) << bits) - UINT32_C(1);
}

static int cmp_x86_processor_by_apic_id(const void* processor_a, const void* processor_b) {
	const uint32_t id_a = ((const struct cpuinfo_x86_processor*) processor_a)->topology.apic_id;
	const uint32_t id_b = ((const struct cpuinfo_x86_processor*) processor_b)->topology.apic_id;

	if (id_a < id_b) {
		return -1;
	} else {
		return id_a > id_b;
	}
}

static void cpuinfo_x86_count_caches(
	const struct cpuinfo_x86_processor* processors,
	uint32_t processors_count,
	uint32_t l1i_count_ptr[restrict static 1],
	uint32_t l1d_count_ptr[restrict static 1],
	uint32_t l2_count_ptr[restrict static 1],
	uint32_t l3_count_ptr[restrict static 1],
	uint32_t l4_count_ptr[restrict static 1])
{
	uint32_t l1i_count = 0, l1d_count = 0, l2_count = 0, l3_count = 0, l4_count = 0;
	uint32_t last_l1i_id = UINT32_MAX, last_l1d_id = UINT32_MAX;
	uint32_t last_l2_id = UINT32_MAX, last_l3_id = UINT32_MAX, last_l4_id = UINT32_MAX;
	for (uint32_t i = 0; i < processors_count; i++) {
		const uint32_t apic_id = processors[i].topology.apic_id;
		if (processors[i].cache.l1i.size != 0) {
			const uint32_t l1i_id = apic_id & ~bit_mask(processors[i].cache.l1i.apic_bits);
			if (l1i_id != last_l1i_id) {
				last_l1i_id = l1i_id;
				l1i_count++;
			}
		}
		if (processors[i].cache.l1d.size != 0) {
			const uint32_t l1d_id = apic_id & ~bit_mask(processors[i].cache.l1d.apic_bits);
			if (l1d_id != last_l1d_id) {
				last_l1d_id = l1d_id;
				l1d_count++;
			}
		}
		if (processors[i].cache.l2.size != 0) {
			const uint32_t l2_id = apic_id & ~bit_mask(processors[i].cache.l2.apic_bits);
			if (l2_id != last_l2_id) {
				last_l2_id = l2_id;
				l2_count++;
			}
		}
		if (processors[i].cache.l3.size != 0) {
			const uint32_t l3_id = apic_id & ~bit_mask(processors[i].cache.l3.apic_bits);
			if (l3_id != last_l3_id) {
				last_l3_id = l3_id;
				l3_count++;
			}
		}
		if (processors[i].cache.l4.size != 0) {
			const uint32_t l4_id = apic_id & ~bit_mask(processors[i].cache.l4.apic_bits);
			if (l4_id != last_l4_id) {
				last_l4_id = l4_id;
				l4_count++;
			}
		}
	}
	*l1i_count_ptr = l1i_count;
	*l1d_count_ptr = l1d_count;
	*l2_count_ptr  = l2_count;
	*l3_count_ptr  = l3_count;
	*l4_count_ptr  = l4_count;
}

static bool cpuinfo_x86_linux_cpulist_callback(uint32_t cpulist_start, uint32_t cpulist_end, void* context) {
	cpu_set_t* cpuset = (cpu_set_t*) context;
	for (uint32_t cpu = cpulist_start; cpu < cpulist_end; cpu++) {
		CPU_SET((int) cpu, cpuset);
	}
	return true;
}

void cpuinfo_x86_linux_init(void) {
	struct cpuinfo_x86_processor* x86_processors = NULL;
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	struct cpuinfo_cache* l3 = NULL;
	struct cpuinfo_cache* l4 = NULL;

	cpu_set_t old_affinity;
	if (sched_getaffinity(0, sizeof(cpu_set_t), &old_affinity) != 0) {
		cpuinfo_log_error("sched_getaffinity failed: %s", strerror(errno));
		return;
	}

	cpu_set_t present_set;
	CPU_ZERO(&present_set);
	cpuinfo_linux_parse_cpulist("/sys/devices/system/cpu/present", cpuinfo_x86_linux_cpulist_callback, &present_set);

	cpu_set_t possible_set;
	CPU_ZERO(&possible_set);
	cpuinfo_linux_parse_cpulist("/sys/devices/system/cpu/possible", cpuinfo_x86_linux_cpulist_callback, &possible_set);

	cpu_set_t processors_set;
	CPU_AND(&processors_set, &present_set, &possible_set);
	const uint32_t processors_count = (uint32_t) CPU_COUNT(&processors_set);
	cpuinfo_log_debug("detected %"PRIu32" logical processors", processors_count);

	x86_processors = calloc(processors_count, sizeof(struct cpuinfo_x86_processor));
	if (x86_processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %d x86 logical processors",
			processors_count * sizeof(struct cpuinfo_x86_processor), processors_count);
		goto cleanup;
	}

	int processor_bit = 0;
	for (int i = 0; i < processors_count; i++, processor_bit++) {
		while (!CPU_ISSET(processor_bit, &processors_set)) {
			processor_bit++;
		}
		cpu_set_t processor_set;
		CPU_ZERO(&processor_set);
		CPU_SET(processor_bit, &processor_set);
		if (sched_setaffinity(0, sizeof(cpu_set_t), &processor_set) != 0) {
			cpuinfo_log_error("sched_setaffinity for processor %d (bit %d) failed: %s",
				i, processor_bit, strerror(errno));
			goto cleanup;
		}

		cpuinfo_x86_init_processor(&x86_processors[i]);
		x86_processors[i].linux_id = processor_bit;
	}

	qsort(x86_processors, (size_t) processors_count, sizeof(struct cpuinfo_x86_processor),
		cmp_x86_processor_by_apic_id);

	processors = calloc((size_t) processors_count, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %d logical processors",
			(size_t) processors_count * sizeof(struct cpuinfo_processor), processors_count);
		goto cleanup;
	}

	for (uint32_t i = 0; i < (uint32_t) processors_count; i++) {
		processors[i].vendor   = x86_processors[i].vendor;
		processors[i].uarch    = x86_processors[i].uarch;
		processors[i].linux_id = x86_processors[i].linux_id;

		/* Initialize topology information */
		const uint32_t apic_id = x86_processors[i].topology.apic_id;
		const uint32_t thread_mask = bit_mask(x86_processors[i].topology.thread_bits_length);
		const uint32_t core_mask   = bit_mask(x86_processors[i].topology.core_bits_length);
		const uint32_t package_offset = max(
			x86_processors[i].topology.thread_bits_offset,
			x86_processors[i].topology.core_bits_offset);
		processors[i].topology = (struct cpuinfo_topology) {
			.thread_id  = (apic_id >> x86_processors[i].topology.thread_bits_offset) & thread_mask,
			.core_id    = (apic_id >> x86_processors[i].topology.core_bits_offset) & core_mask,
			.package_id = apic_id >> package_offset,
			.apic_id    = x86_processors[i].topology.apic_id,
		};
	}

	uint32_t l1i_count = 0, l1d_count = 0, l2_count = 0, l3_count = 0, l4_count = 0;
	cpuinfo_x86_count_caches(x86_processors, processors_count,
		&l1i_count, &l1d_count, &l2_count, &l3_count, &l4_count);

	cpuinfo_log_info("detected %"PRIu32" L1I caches", l1i_count);
	cpuinfo_log_info("detected %"PRIu32" L1D caches", l1d_count);
	cpuinfo_log_info("detected %"PRIu32" L2 caches", l2_count);
	cpuinfo_log_info("detected %"PRIu32" L3 caches", l3_count);
	cpuinfo_log_info("detected %"PRIu32" L4 caches", l4_count);

	if (l1i_count != 0) {
		l1i = calloc(l1i_count, sizeof(struct cpuinfo_cache));
		if (l1i == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
				l1i_count * sizeof(struct cpuinfo_cache), l1i_count);
			goto cleanup;
		}
	}
	if (l1d_count != 0) {
		l1d = calloc(l1d_count, sizeof(struct cpuinfo_cache));
		if (l1d == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
				l1d_count * sizeof(struct cpuinfo_cache), l1d_count);
			goto cleanup;
		}
	}
	if (l2_count != 0) {
		l2 = calloc(l2_count, sizeof(struct cpuinfo_cache));
		if (l2 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2_count * sizeof(struct cpuinfo_cache), l2_count);
			goto cleanup;
		}
	}
	if (l3_count != 0) {
		l3 = calloc(l3_count, sizeof(struct cpuinfo_cache));
		if (l3 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L3 caches",
				l3_count * sizeof(struct cpuinfo_cache), l3_count);
			goto cleanup;
		}
	}
	if (l4_count != 0) {
		l4 = calloc(l4_count, sizeof(struct cpuinfo_cache));
		if (l4 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L4 caches",
				l4_count * sizeof(struct cpuinfo_cache), l4_count);
			goto cleanup;
		}
	}

	uint32_t l1i_index = 0, l1d_index = 0, l2_index = 0, l3_index = 0, l4_index = 0;
	uint32_t last_l1i_id = UINT32_MAX, last_l1d_id = UINT32_MAX;
	uint32_t last_l2_id = UINT32_MAX, last_l3_id = UINT32_MAX, last_l4_id = UINT32_MAX;
	for (uint32_t i = 0; i < processors_count; i++) {
		const uint32_t apic_id = processors[i].topology.apic_id;
		if (x86_processors[i].cache.l1i.size != 0) {
			const uint32_t l1i_id = apic_id & ~bit_mask(x86_processors[i].cache.l1i.apic_bits);
			processors[i].cache.l1i = &l1i[l1i_index];
			if (l1i_id != last_l1i_id) {
				/* new cache */
				last_l1i_id = l1i_id;
				l1i[l1i_index++] = (struct cpuinfo_cache) {
					.size            = x86_processors[i].cache.l1i.size,
					.associativity   = x86_processors[i].cache.l1i.associativity,
					.sets            = x86_processors[i].cache.l1i.sets,
					.partitions      = x86_processors[i].cache.l1i.partitions,
					.line_size       = x86_processors[i].cache.l1i.line_size,
					.flags           = x86_processors[i].cache.l1i.flags,
					.processor_start = i,
					.processor_count = 1,
				};
			} else {
				/* another processor sharing the same cache */
				l1i[l1i_index - 1].processor_count += 1;
			}
		} else {
			/* reset cache id */
			last_l1i_id = UINT32_MAX;
		}
		if (x86_processors[i].cache.l1d.size != 0) {
			const uint32_t l1d_id = apic_id & ~bit_mask(x86_processors[i].cache.l1d.apic_bits);
			processors[i].cache.l1d = &l1d[l1d_index];
			if (l1d_id != last_l1d_id) {
				/* new cache */
				last_l1d_id = l1d_id;
				l1d[l1d_index++] = (struct cpuinfo_cache) {
					.size            = x86_processors[i].cache.l1d.size,
					.associativity   = x86_processors[i].cache.l1d.associativity,
					.sets            = x86_processors[i].cache.l1d.sets,
					.partitions      = x86_processors[i].cache.l1d.partitions,
					.line_size       = x86_processors[i].cache.l1d.line_size,
					.flags           = x86_processors[i].cache.l1d.flags,
					.processor_start = i,
					.processor_count = 1,
				};
			} else {
				/* another processor sharing the same cache */
				l1d[l1d_index - 1].processor_count += 1;
			}
		} else {
			/* reset cache id */
			last_l1d_id = UINT32_MAX;
		}
		if (x86_processors[i].cache.l2.size != 0) {
			const uint32_t l2_id = apic_id & ~bit_mask(x86_processors[i].cache.l2.apic_bits);
			processors[i].cache.l2 = &l2[l2_index];
			if (l2_id != last_l2_id) {
				/* new cache */
				last_l2_id = l2_id;
				l2[l2_index++] = (struct cpuinfo_cache) {
					.size            = x86_processors[i].cache.l2.size,
					.associativity   = x86_processors[i].cache.l2.associativity,
					.sets            = x86_processors[i].cache.l2.sets,
					.partitions      = x86_processors[i].cache.l2.partitions,
					.line_size       = x86_processors[i].cache.l2.line_size,
					.flags           = x86_processors[i].cache.l2.flags,
					.processor_start = i,
					.processor_count = 1,
				};
			} else {
				/* another processor sharing the same cache */
				l2[l2_index - 1].processor_count += 1;
			}
		} else {
			/* reset cache id */
			last_l2_id = UINT32_MAX;
		}
		if (x86_processors[i].cache.l3.size != 0) {
			const uint32_t l3_id = apic_id & ~bit_mask(x86_processors[i].cache.l3.apic_bits);
			processors[i].cache.l3 = &l3[l3_index];
			if (l3_id != last_l3_id) {
				/* new cache */
				last_l3_id = l3_id;
				l3[l3_index++] = (struct cpuinfo_cache) {
					.size            = x86_processors[i].cache.l3.size,
					.associativity   = x86_processors[i].cache.l3.associativity,
					.sets            = x86_processors[i].cache.l3.sets,
					.partitions      = x86_processors[i].cache.l3.partitions,
					.line_size       = x86_processors[i].cache.l3.line_size,
					.flags           = x86_processors[i].cache.l3.flags,
					.processor_start = i,
					.processor_count = 1,
				};
			} else {
				/* another processor sharing the same cache */
				l3[l3_index - 1].processor_count += 1;
			}
		} else {
			/* reset cache id */
			last_l3_id = UINT32_MAX;
		}
		if (x86_processors[i].cache.l4.size != 0) {
			const uint32_t l4_id = apic_id & ~bit_mask(x86_processors[i].cache.l4.apic_bits);
			processors[i].cache.l4 = &l4[l4_index];
			if (l4_id != last_l4_id) {
				/* new cache */
				last_l4_id = l4_id;
				l4[l4_index++] = (struct cpuinfo_cache) {
					.size            = x86_processors[i].cache.l4.size,
					.associativity   = x86_processors[i].cache.l4.associativity,
					.sets            = x86_processors[i].cache.l4.sets,
					.partitions      = x86_processors[i].cache.l4.partitions,
					.line_size       = x86_processors[i].cache.l4.line_size,
					.flags           = x86_processors[i].cache.l4.flags,
					.processor_start = i,
					.processor_count = 1,
				};
			} else {
				/* another processor sharing the same cache */
				l4[l4_index - 1].processor_count += 1;
			}
		} else {
			/* reset cache id */
			last_l4_id = UINT32_MAX;
		}
	}

	/* Commit changes */
	cpuinfo_processors = processors;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;
	cpuinfo_cache[cpuinfo_cache_level_3]  = l3;
	cpuinfo_cache[cpuinfo_cache_level_4]  = l4;

	cpuinfo_processors_count = processors_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = l1i_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = l1d_count;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2_count;
	cpuinfo_cache_count[cpuinfo_cache_level_3]  = l3_count;
	cpuinfo_cache_count[cpuinfo_cache_level_4]  = l4_count;

	processors = NULL;
	l1i = l1d = l2 = l3 = l4 = NULL;

cleanup:
	if (sched_setaffinity(0, sizeof(cpu_set_t), &old_affinity) != 0) {
		cpuinfo_log_warning("could not restore initial process affinity: "
			"sched_getaffinity failed: %s", strerror(errno));
	}

	free(x86_processors);
	free(processors);
	free(l1i);
	free(l1d);
	free(l2);
	free(l3);
	free(l4);
}
