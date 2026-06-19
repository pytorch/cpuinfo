#include <stdlib.h>
#include <string.h>
#include <zircon/syscalls.h>

#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <x86/api.h>

static inline uint32_t max(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}

static inline uint32_t bit_mask(uint32_t bits) {
	return (UINT32_C(1) << bits) - UINT32_C(1);
}

void cpuinfo_x86_fuchsia_init(void) {
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_package* packages = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	struct cpuinfo_cache* l3 = NULL;
	struct cpuinfo_cache* l4 = NULL;

	/* Total logical processors, as available from Fuchsia kernel API */
	uint32_t fuchsia_topology_processors = zx_system_get_num_cpus();

	/*
	 * Since Fuchsia API does not expose total physical processors, assume them to
	 * be the same as the total logical processors.
	 */
	uint32_t fuchsia_topology_cores = fuchsia_topology_processors;

	/* Total physical packages assumed to be on x86 devices with Fuchsia */
	uint32_t fuchsia_topology_packages = 1;

	processors = calloc(fuchsia_topology_processors, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate bytes for descriptions of logical processors");
		goto cleanup;
	}
	cores = calloc(fuchsia_topology_cores, sizeof(struct cpuinfo_core));
	if (cores == NULL) {
		cpuinfo_log_error("failed to allocate bytes for descriptions of cores");
		goto cleanup;
	}
	/* On x86 cluster of cores is a physical package */
	clusters = calloc(fuchsia_topology_packages, sizeof(struct cpuinfo_cluster));
	if (clusters == NULL) {
		cpuinfo_log_error("failed to allocate bytes for descriptions of core clusters");
		goto cleanup;
	}
	packages = calloc(fuchsia_topology_packages, sizeof(struct cpuinfo_package));
	if (packages == NULL) {
		cpuinfo_log_error("failed to allocate bytes for descriptions of physical packages");
		goto cleanup;
	}

	struct cpuinfo_x86_processor x86_processor;
	memset(&x86_processor, 0, sizeof(x86_processor));
	cpuinfo_x86_init_processor(&x86_processor);
	char brand_string[48];
	cpuinfo_x86_normalize_brand_string(x86_processor.brand_string, brand_string);

	const uint32_t threads_per_core = fuchsia_topology_processors / fuchsia_topology_cores;
	const uint32_t threads_per_package = fuchsia_topology_processors / fuchsia_topology_packages;
	const uint32_t cores_per_package = fuchsia_topology_cores / fuchsia_topology_packages;
	for (uint32_t i = 0; i < fuchsia_topology_packages; i++) {
		clusters[i] = (struct cpuinfo_cluster){
				.processor_start = i * threads_per_package,
				.processor_count = threads_per_package,
				.core_start = i * cores_per_package,
				.core_count = cores_per_package,
				.cluster_id = 0,
				.package = packages + i,
				.vendor = x86_processor.vendor,
				.uarch = x86_processor.uarch,
				.cpuid = x86_processor.cpuid,
		};
		packages[i].processor_start = i * threads_per_package;
		packages[i].processor_count = threads_per_package;
		packages[i].core_start = i * cores_per_package;
		packages[i].core_count = cores_per_package;
		packages[i].cluster_start = i;
		packages[i].cluster_count = 1;
		cpuinfo_x86_format_package_name(x86_processor.vendor, brand_string, packages[i].name);
	}
	for (uint32_t i = 0; i < fuchsia_topology_cores; i++) {
		cores[i] = (struct cpuinfo_core){
				.processor_start = i * threads_per_core,
				.processor_count = threads_per_core,
				.core_id = i % cores_per_package,
				.cluster = clusters + i / cores_per_package,
				.package = packages + i / cores_per_package,
				.vendor = x86_processor.vendor,
				.uarch = x86_processor.uarch,
				.cpuid = x86_processor.cpuid,
		};
	}
	for (uint32_t i = 0; i < fuchsia_topology_processors; i++) {
		const uint32_t smt_id = i % threads_per_core;
		const uint32_t core_id = i / threads_per_core;
		const uint32_t package_id = i / threads_per_package;

		/* Reconstruct APIC IDs from topology components */
		const uint32_t thread_bits_mask = bit_mask(x86_processor.topology.thread_bits_length);
		const uint32_t core_bits_mask = bit_mask(x86_processor.topology.core_bits_length);
		const uint32_t package_bits_offset =
				max(x86_processor.topology.thread_bits_offset + x86_processor.topology.thread_bits_length,
						x86_processor.topology.core_bits_offset + x86_processor.topology.core_bits_length);
		const uint32_t apic_id = ((smt_id & thread_bits_mask) << x86_processor.topology.thread_bits_offset) |
														 ((core_id & core_bits_mask) << x86_processor.topology.core_bits_offset) |
														 (package_id << package_bits_offset);
		cpuinfo_log_debug("reconstructed APIC ID 0x%08"PRIx32" for thread %"PRIu32, apic_id, i);

		processors[i].smt_id = smt_id;
		processors[i].core = cores + i / threads_per_core;
		processors[i].cluster = clusters + i / threads_per_package;
		processors[i].package = packages + i / threads_per_package;
		processors[i].apic_id = apic_id;
	}

	/* L1 caches */
	uint32_t threads_per_l1 = 0, l1_count = 0;
	if (x86_processor.cache.l1i.size != 0 || x86_processor.cache.l1d.size != 0) {
		/* Each core has an L1 cache on currently supported Fuchsia devices */
		l1_count = fuchsia_topology_cores;
		if (threads_per_l1 == 0) {
		  threads_per_l1 = fuchsia_topology_processors / l1_count;
		}
		cpuinfo_log_debug("detected %"PRIu32" L1 caches", l1_count);
	}

	if (x86_processor.cache.l1i.size != 0) {
		l1i = calloc(l1_count, sizeof(struct cpuinfo_cache));
		if (l1i == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
				l1_count * sizeof(struct cpuinfo_cache), l1_count);
			return;
		}
		for (uint32_t c = 0; c < l1_count; c++) {
			l1i[c] = (struct cpuinfo_cache) {
				.size            = x86_processor.cache.l1i.size,
				.associativity   = x86_processor.cache.l1i.associativity,
				.sets            = x86_processor.cache.l1i.sets,
				.partitions      = x86_processor.cache.l1i.partitions,
				.line_size       = x86_processor.cache.l1i.line_size,
				.flags           = x86_processor.cache.l1i.flags,
				.processor_start = c * threads_per_l1,
				.processor_count = threads_per_l1,
			};
		}
		for (uint32_t t = 0; t < fuchsia_topology_processors; t++) {
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
				.size            = x86_processor.cache.l1d.size,
				.associativity   = x86_processor.cache.l1d.associativity,
				.sets            = x86_processor.cache.l1d.sets,
				.partitions      = x86_processor.cache.l1d.partitions,
				.line_size       = x86_processor.cache.l1d.line_size,
				.flags           = x86_processor.cache.l1d.flags,
				.processor_start = c * threads_per_l1,
				.processor_count = threads_per_l1,
			};
		}
		for (uint32_t t = 0; t < fuchsia_topology_processors; t++) {
			processors[t].cache.l1d = &l1d[t / threads_per_l1];
		}
	}

	/* L2 cache */
	uint32_t threads_per_l2 = 0, l2_count = 0;
	if (x86_processor.cache.l2.size != 0) {
		/* Current Fuchsia devices share a unified L2 cache */
		threads_per_l2 = fuchsia_topology_processors;
		l2_count = fuchsia_topology_processors / threads_per_l2;
		cpuinfo_log_debug("detected %"PRIu32" L2 caches", l2_count);
	}
	if (l2_count != 0) {
		l2 = calloc(l2_count, sizeof(struct cpuinfo_cache));
		if (l2 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2_count * sizeof(struct cpuinfo_cache), l2_count);
			return;
		}
		for (uint32_t c = 0; c < l2_count; c++) {
			l2[c] = (struct cpuinfo_cache) {
				.size            = x86_processor.cache.l2.size,
				.associativity   = x86_processor.cache.l2.associativity,
				.sets            = x86_processor.cache.l2.sets,
				.partitions      = x86_processor.cache.l2.partitions,
				.line_size       = x86_processor.cache.l2.line_size,
				.flags           = x86_processor.cache.l2.flags,
				.processor_start = c * threads_per_l2,
				.processor_count = threads_per_l2,
			};
		}
		for (uint32_t t = 0; t < fuchsia_topology_processors; t++) {
			processors[t].cache.l2 = &l2[t / threads_per_l2];
		}
	}

	/* L3 and L4 cache are not present currently on any supported Fuchsia devices */
	uint32_t l3_count = 0, l4_count = 0;

	/* Commit changes */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
	cpuinfo_clusters = clusters;
	cpuinfo_packages = packages;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;
	cpuinfo_cache[cpuinfo_cache_level_3]  = l3;
	cpuinfo_cache[cpuinfo_cache_level_4]  = l4;

	cpuinfo_processors_count = fuchsia_topology_processors;
	cpuinfo_cores_count = fuchsia_topology_cores;
	cpuinfo_clusters_count = fuchsia_topology_packages;
	cpuinfo_packages_count = fuchsia_topology_packages;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = l1_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = l1_count;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2_count;
	cpuinfo_cache_count[cpuinfo_cache_level_3]  = l3_count;
	cpuinfo_cache_count[cpuinfo_cache_level_4]  = l4_count;
	cpuinfo_max_cache_size = cpuinfo_compute_max_cache_size(&processors[0]);

	cpuinfo_global_uarch = (struct cpuinfo_uarch_info) {
		.uarch = x86_processor.uarch,
		.cpuid = x86_processor.cpuid,
		.processor_count = fuchsia_topology_processors,
		.core_count = fuchsia_topology_cores,
	};

	__sync_synchronize();
	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;
	clusters = NULL;
	packages = NULL;
	l1i = l1d = l2 = l3 = l4 = NULL;

cleanup:
	free(processors);
	free(cores);
	free(clusters);
	free(packages);
	free(l1i);
	free(l1d);
	free(l2);
	free(l3);
	free(l4);
}
