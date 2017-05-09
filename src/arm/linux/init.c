#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo.h>
#include <arm/linux/api.h>
#include <arm/api.h>
#include <linux/api.h>
#include <api.h>
#include <log.h>


struct cpuinfo_arm_isa cpuinfo_isa = { 0 };


void cpuinfo_arm_linux_init(void) {
	uint32_t proc_cpuinfo_count = 0;
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	uint32_t processors_count = 0;
	uint32_t l1i_count = 0;
	uint32_t l1d_count = 0;
	uint32_t l2_count = 0;

	struct proc_cpuinfo* proc_cpuinfo_entries = cpuinfo_arm_linux_parse_proc_cpuinfo(&proc_cpuinfo_count);

	if (proc_cpuinfo_count != 0) {
		#if CPUINFO_ARCH_ARM
			cpuinfo_arm_linux_decode_isa_from_proc_cpuinfo(
				proc_cpuinfo_entries, proc_cpuinfo_count, &cpuinfo_isa);
		#elif CPUINFO_ARCH_ARM64
			cpuinfo_arm64_linux_decode_isa_from_proc_cpuinfo(
				proc_cpuinfo_entries, &cpuinfo_isa);
		#endif
		processors_count = proc_cpuinfo_count;

		processors = calloc(processors_count, sizeof(struct cpuinfo_processor));
		if (processors == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
				proc_cpuinfo_count * sizeof(struct cpuinfo_processor), proc_cpuinfo_count);
			goto cleanup;
		}
		for (uint32_t i = 0; i < proc_cpuinfo_count; i++) {
			cpuinfo_arm_decode_vendor_uarch(
				proc_cpuinfo_entries[i].implementer,
				proc_cpuinfo_entries[i].part,
#if CPUINFO_ARCH_ARM
				!!(proc_cpuinfo_entries[i].features & PROC_CPUINFO_FEATURE_VFPV4),
#endif
				&processors[i].vendor, &processors[i].uarch);
			processors[i].topology = (struct cpuinfo_topology) {
				.thread_id = 0,
				.core_id = i,
				.package_id = 0,
				.linux_id = (int) i
			};
		}

		/*
		 * Assumptions:
		 * - At most 2 cache levels
		 * - Either all or no cores have L1I/L1D/L2 cache.
		 * - If present, L1 cache is private to the core.
		 * - If present, L2 cache is shared between all cores.
		 */
		struct cpuinfo_cache private_l1i = { 0 };
		struct cpuinfo_cache private_l1d = { 0 };
		struct cpuinfo_cache shared_l2 = { 0 };
		cpuinfo_arm_decode_cache(
			processors[0].uarch,
			proc_cpuinfo_count,
			proc_cpuinfo_entries[0].part,
			proc_cpuinfo_entries[0].architecture.version,
			&private_l1i, &private_l1d, &shared_l2);
		if (private_l1i.size != 0) {
			l1i_count = proc_cpuinfo_count;
		}
		if (private_l1d.size != 0) {
			l1d_count = proc_cpuinfo_count;
			if (shared_l2.size != 0) {
				l2_count = 1;
			}
		}

		cpuinfo_log_info("detected %"PRIu32" L1I caches", l1i_count);
		cpuinfo_log_info("detected %"PRIu32" L1D caches", l1d_count);
		cpuinfo_log_info("detected %"PRIu32" L2 caches", l2_count);

		if (l1i_count != 0) {
			l1i = malloc(l1i_count * sizeof(struct cpuinfo_cache));
			if (l1i == NULL) {
				cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
					l1i_count * sizeof(struct cpuinfo_cache), l1i_count);
				goto cleanup;
			}
			for (uint32_t i = 0; i < l1i_count; i++) {
				/* L1I reported in /proc/cpuinfo overrides defaults */
				#if CPUINFO_ARCH_ARM
					if ((proc_cpuinfo_entries[i].valid_mask & PROC_CPUINFO_VALID_ICACHE) == PROC_CPUINFO_VALID_ICACHE) {
						l1i[i] = (struct cpuinfo_cache) {
							.size = proc_cpuinfo_entries[i].cache.i_size,
							.associativity = proc_cpuinfo_entries[i].cache.i_assoc,
							.sets = proc_cpuinfo_entries[i].cache.i_sets,
							.partitions = 1,
							.line_size = proc_cpuinfo_entries[i].cache.i_line_length
						};
					} else {
						cpuinfo_arm_decode_cache(
							processors[i].uarch,
							proc_cpuinfo_count,
							proc_cpuinfo_entries[i].part,
							proc_cpuinfo_entries[i].architecture.version,
							&l1i[i], &private_l1d, &shared_l2);
					}
				#elif CPUINFO_ARCH_ARM64
					cpuinfo_arm_decode_cache(
						processors[i].uarch,
						proc_cpuinfo_count,
						proc_cpuinfo_entries[i].part,
						proc_cpuinfo_entries[i].architecture.version,
						&l1i[i], &private_l1d, &shared_l2);
				#endif
				l1i[i].thread_start = i;
				l1i[i].thread_count = 1;
			}
		}
		if (l1d_count != 0) {
			l1d = malloc(l1d_count * sizeof(struct cpuinfo_cache));
			if (l1d == NULL) {
				cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
					l1d_count * sizeof(struct cpuinfo_cache), l1d_count);
				goto cleanup;
			}
			for (uint32_t i = 0; i < l1d_count; i++) {
				#if CPUINFO_ARCH_ARM
					/* L1D reported in /proc/cpuinfo overrides defaults */
					if ((proc_cpuinfo_entries[i].valid_mask & PROC_CPUINFO_VALID_DCACHE) == PROC_CPUINFO_VALID_DCACHE) {
						l1d[i] = (struct cpuinfo_cache) {
							.size = proc_cpuinfo_entries[i].cache.d_size,
							.associativity = proc_cpuinfo_entries[i].cache.d_assoc,
							.sets = proc_cpuinfo_entries[i].cache.d_sets,
							.partitions = 1,
							.line_size = proc_cpuinfo_entries[i].cache.d_line_length
						};
					} else {
						cpuinfo_arm_decode_cache(
							processors[i].uarch,
							proc_cpuinfo_count,
							proc_cpuinfo_entries[i].part,
							proc_cpuinfo_entries[i].architecture.version,
							&private_l1i, &l1d[i], &shared_l2);
					}
				#elif CPUINFO_ARCH_ARM64
					cpuinfo_arm_decode_cache(
						processors[i].uarch,
						proc_cpuinfo_count,
						proc_cpuinfo_entries[i].part,
						proc_cpuinfo_entries[i].architecture.version,
						&private_l1i, &l1d[i], &shared_l2);
				#endif
				l1d[i].thread_start = i;
				l1d[i].thread_count = 1;
			}
		}
		if (l2_count != 0) {
			l2 = malloc(l2_count * sizeof(struct cpuinfo_cache));
			if (l2 == NULL) {
				cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
					l2_count * sizeof(struct cpuinfo_cache), l2_count);
				goto cleanup;
			}
			/* L2 cache is never reported in /proc/cpuinfo; use defaults */
			*l2 = shared_l2;
			l2->thread_start = 0;
			l2->thread_count = proc_cpuinfo_count;
		}
	}

	/* Commit */
	cpuinfo_processors = processors;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;

	cpuinfo_processors_count = processors_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = l1i_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = l1d_count;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2_count;

	processors = NULL;
	l1i = l1d = l2 = NULL;

cleanup:
	free(processors);
	free(l1i);
	free(l1d);
	free(l2);
	free(proc_cpuinfo_entries);
}
