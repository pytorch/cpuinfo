#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <errno.h>

#include <zircon/syscalls.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

struct cpuinfo_arm_isa cpuinfo_isa = { 0 };

void cpuinfo_arm_fuchsia_init(void) {
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_package* packages = NULL;
	struct cpuinfo_uarch_info* uarchs = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	struct cpuinfo_cache* l3 = NULL;

	/* Total logical processors, as available from Fuchsia kernel API */
	uint32_t fuchsia_topology_processors = zx_system_get_num_cpus();

	/*
	 * Since Fuchsia API does not expose total physical processors, assume them to
	 * be the same as the total logical processors.
	 */
	uint32_t fuchsia_topology_cores = fuchsia_topology_processors;

	/* Total physical packages assumed to be on arm devices with Fuchsia */
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
	packages = calloc(fuchsia_topology_packages, sizeof(struct cpuinfo_package));
	if (packages == NULL) {
		cpuinfo_log_error("failed to allocate bytes for descriptions of physical packages");
		goto cleanup;
	}

	const uint32_t threads_per_core = fuchsia_topology_processors / fuchsia_topology_cores;
	const uint32_t threads_per_package = fuchsia_topology_processors / fuchsia_topology_packages;
	const uint32_t cores_per_package = fuchsia_topology_cores / fuchsia_topology_packages;

	for (uint32_t i = 0; i < fuchsia_topology_packages; i++) {
		packages[i] = (struct cpuinfo_package) {
			.processor_start = i * threads_per_package,
			.processor_count = threads_per_package,
			.core_start = i * cores_per_package,
			.core_count = cores_per_package,
		};
	}

	uint32_t num_clusters = 1;
	for (uint32_t i = 0; i < fuchsia_topology_cores; i++) {
		cores[i] = (struct cpuinfo_core) {
			.processor_start = i * threads_per_core,
			.processor_count = threads_per_core,
			.core_id = i % cores_per_package,
			.package = packages + i / cores_per_package,
		};
		if (i != 0 && cores[i].uarch != cores[i - 1].uarch) {
			num_clusters++;
		}
	}
	for (uint32_t i = 0; i < fuchsia_topology_processors; i++) {
		const uint32_t smt_id = i % threads_per_core;
		const uint32_t core_id = i / threads_per_core;
		const uint32_t package_id = i / threads_per_package;

		processors[i].smt_id = smt_id;
		processors[i].core = &cores[core_id];
		processors[i].package = &packages[package_id];
	}

	clusters = calloc(num_clusters, sizeof(struct cpuinfo_cluster));
	if (clusters == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" clusters",
			num_clusters * sizeof(struct cpuinfo_cluster), num_clusters);
		goto cleanup;
	}

	uarchs = calloc(num_clusters, sizeof(struct cpuinfo_uarch_info));
	if (uarchs == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" uarchs",
			num_clusters * sizeof(enum cpuinfo_uarch), num_clusters);
		goto cleanup;
	}
	uint32_t cluster_idx = UINT32_MAX;
	for (uint32_t i = 0; i < fuchsia_topology_cores; i++) {
		if (i == 0 || cores[i].uarch != cores[i - 1].uarch) {
			cluster_idx++;
			uarchs[cluster_idx] = (struct cpuinfo_uarch_info) {
				.uarch = cores[i].uarch,
				.processor_count = 1,
				.core_count = 1,
			};
			clusters[cluster_idx] = (struct cpuinfo_cluster) {
				.processor_start = i * threads_per_core,
				.processor_count = 1,
				.core_start = i,
				.core_count = 1,
				.cluster_id = cluster_idx,
				.package = cores[i].package,
				.vendor = cores[i].vendor,
				.uarch = cores[i].uarch,
			};
		} else {
			uarchs[cluster_idx].processor_count++;
			uarchs[cluster_idx].core_count++;
			clusters[cluster_idx].processor_count++;
			clusters[cluster_idx].core_count++;
		}
		cores[i].cluster = &clusters[cluster_idx];
	}

	for (uint32_t i = 0; i < fuchsia_topology_processors; i++) {
		const uint32_t core_id = i / threads_per_core;
		processors[i].cluster = cores[core_id].cluster;
	}

	for (uint32_t i = 0; i < fuchsia_topology_packages; i++) {
		packages[i].cluster_start = 0;
		packages[i].cluster_count = num_clusters;
	}

	/* Cache related information for currently supported Fuchsia ARM devices */
	const uint32_t cacheline_size = zx_system_get_dcache_line_size();
	const uint32_t l1d_cache_size = 1;
	const uint32_t l1i_cache_size = 1;
	const uint32_t l2_cache_size = 512;
	const uint32_t cache_partitions = 1;
	const uint32_t cache_flags = 0;

	/* L1 cache */
	uint32_t threads_per_l1 = 0, l1_count = 0;
	if (l1i_cache_size != 0 || l1d_cache_size != 0) {
		/* Assume L1 caches are private to each core */
		threads_per_l1 = 1;
		l1_count = fuchsia_topology_processors / threads_per_l1;
		cpuinfo_log_debug("detected %"PRIu32" L1 caches", l1_count);
	}

	if (l1i_cache_size != 0) {
		l1i = calloc(l1_count, sizeof(struct cpuinfo_cache));
		if (l1i == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
				l1_count * sizeof(struct cpuinfo_cache), l1_count);
			goto cleanup;
		}
		for (uint32_t c = 0; c < l1_count; c++) {
			l1i[c] = (struct cpuinfo_cache) {
				.size            = l1i_cache_size,
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
				.processor_start = c * threads_per_l1,
				.processor_count = threads_per_l1,
			};
		}
		for (uint32_t t = 0; t < fuchsia_topology_processors; t++) {
			processors[t].cache.l1i = &l1i[t / threads_per_l1];
		}
	}

	if (l1d_cache_size != 0) {
		l1d = calloc(l1_count, sizeof(struct cpuinfo_cache));
		if (l1d == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
				l1_count * sizeof(struct cpuinfo_cache), l1_count);
			goto cleanup;
		}
		for (uint32_t c = 0; c < l1_count; c++) {
			l1d[c] = (struct cpuinfo_cache) {
				.size            = l1d_cache_size,
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
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
	if (l2_cache_size != 0) {
		/* Assume L2 cache is shared between all cores */
		threads_per_l2 = fuchsia_topology_cores;
		l2_count = 1;
		cpuinfo_log_debug("detected %"PRIu32" L2 caches", l2_count);
	}

	if (l2_count != 0) {
		l2 = calloc(l2_count, sizeof(struct cpuinfo_cache));
		if (l2 == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2_count * sizeof(struct cpuinfo_cache), l2_count);
			goto cleanup;
		}
		for (uint32_t c = 0; c < l2_count; c++) {
			l2[c] = (struct cpuinfo_cache) {
				.size            = l2_cache_size,
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
				.processor_start = c * threads_per_l2,
				.processor_count = threads_per_l2,
			};
		}
		for (uint32_t t = 0; t < fuchsia_topology_processors; t++) {
			processors[t].cache.l2 = &l2[0];
		}
	}

	/* L3 cache don't exist on currently supported Fuchsia ARM devices */
	uint32_t l3_count = 0;

	/* Commit changes */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
	cpuinfo_clusters = clusters;
	cpuinfo_packages = packages;
	cpuinfo_uarchs = uarchs;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;
	cpuinfo_cache[cpuinfo_cache_level_3]  = l3;

	cpuinfo_processors_count = fuchsia_topology_processors;
	cpuinfo_cores_count = fuchsia_topology_cores;
	cpuinfo_clusters_count = num_clusters;
	cpuinfo_packages_count = fuchsia_topology_packages;
	cpuinfo_uarchs_count = num_clusters;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = l1_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = l1_count;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2_count;
	cpuinfo_cache_count[cpuinfo_cache_level_3]  = l3_count;
	cpuinfo_max_cache_size = cpuinfo_compute_max_cache_size(&processors[0]);

	__sync_synchronize();

	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;
	clusters = NULL;
	packages = NULL;
	uarchs = NULL;
	l1i = l1d = l2 = l3 = NULL;

cleanup:
	free(processors);
	free(cores);
	free(clusters);
	free(packages);
	free(uarchs);
	free(l1i);
	free(l1d);
	free(l2);
	free(l3);
}
