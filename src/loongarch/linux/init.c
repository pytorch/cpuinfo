#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo.h>
#include <loongarch/linux/api.h>
#include <loongarch/api.h>
#include <loongarch/cpucfg.h>
#include <loongarch/prid.h>
#include <linux/api.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>


struct cpuinfo_loongarch_isa cpuinfo_isa = { 0 };

static inline bool bitmask_all(uint32_t bitfield, uint32_t mask) {
	return (bitfield & mask) == mask;
}

static inline uint32_t min(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}

static inline int cmp(uint32_t a, uint32_t b) {
	return (a > b) - (a < b);
}

static int cmp_loongarch_linux_processor(const void* ptr_a, const void* ptr_b) {
	const struct cpuinfo_loongarch_linux_processor* processor_a = (const struct cpuinfo_loongarch_linux_processor*) ptr_a;
	const struct cpuinfo_loongarch_linux_processor* processor_b = (const struct cpuinfo_loongarch_linux_processor*) ptr_b;

	/* Move usable processors towards the start of the array */
	const bool usable_a = bitmask_all(processor_a->flags, CPUINFO_LINUX_FLAG_VALID);
	const bool usable_b = bitmask_all(processor_b->flags, CPUINFO_LINUX_FLAG_VALID);
	if (usable_a != usable_b) {
		return (int) usable_b - (int) usable_a;
	}

	/* Compare based on processsor ID (i.e. processor 0 < processor 1) */
	const uint32_t pro_a = processor_a->system_processor_id;
	const uint32_t pro_b = processor_b->system_processor_id;

	return cmp(pro_a, pro_b);
}

static inline bool is_cache_bit_set(enum cache_baseinfo_bit bit, uint32_t info) {
	return !!((info >> bit) & 1);
}

static inline bool set_cpuinfo_cache_by_cpucfg(
	enum cpucfg_regs reg,
	struct cpuinfo_cache cache[restrict static 1])
{
	uint32_t data;

	if (!cpucfg(reg, &data)) {
		return false;
	}

	*cache = (struct cpuinfo_cache) {
		.associativity = ((data & CACHE_WAYS_MASK) >> CACHE_WAYS_OFFSET) + 1,
		.sets = 1 << ((data & CACHE_SETS_MASK) >> CACHE_SETS_OFFSET),
		.line_size = 1 << ((data & CACHE_LSIZE_MASK) >> CACHE_LSIZE_OFFSET),
		.partitions = 1,
	};
	cache->size = cache->associativity * cache->sets * cache->line_size;
	return true;
}

static void try_set_cache_by_cpucfg(
	struct cpuinfo_loongarch_linux_processor *processors,
	uint32_t count)
{
	uint32_t info, flags = 0;
	struct cpuinfo_cache l1i, l1d, l2, l3;

	if (!cpucfg(CPUCFG_REG_CACHE_BASEINFO, &info)) {
		return;
	}

	if (is_cache_bit_set(L1_IU_Present, info) && !is_cache_bit_set(L1_IU_Unify, info)) {
		if (set_cpuinfo_cache_by_cpucfg(CPUCFG_REG_CACHE_L1_IU, &l1i)) {
			flags |= CPUINFO_LOONGARCH_LINUX_VALID_L1I;
		}
	}
	if (is_cache_bit_set(L1_D_Present, info)) {
		if (set_cpuinfo_cache_by_cpucfg(CPUCFG_REG_CACHE_L1_D, &l1d)) {
			flags |= CPUINFO_LOONGARCH_LINUX_VALID_L1D;
		}
	}
	if (is_cache_bit_set(L2_IU_Present, info) && is_cache_bit_set(L2_IU_Unify, info)) {
		if (set_cpuinfo_cache_by_cpucfg(CPUCFG_REG_CACHE_L2_IU, &l2)) {
			flags |= CPUINFO_LOONGARCH_LINUX_VALID_L2;
		}
	}
	if (is_cache_bit_set(L3_IU_Present, info) && is_cache_bit_set(L3_IU_Unify, info)) {
		if (set_cpuinfo_cache_by_cpucfg(CPUCFG_REG_CACHE_L3_IU, &l3)) {
			flags |= CPUINFO_LOONGARCH_LINUX_VALID_L3;
		}
	}

	for (uint32_t i = 0; i < count; i++) {
		if (flags & CPUINFO_LOONGARCH_LINUX_VALID_L1I) {
			processors[i].l1i = l1i;
		}
		if (flags & CPUINFO_LOONGARCH_LINUX_VALID_L1D) {
			processors[i].l1d = l1d;
		}
		if (flags & CPUINFO_LOONGARCH_LINUX_VALID_L2) {
			processors[i].l2 = l2;
		}
		if (flags & CPUINFO_LOONGARCH_LINUX_VALID_L3) {
			processors[i].l3 = l3;
		}
		processors[i].flags |= flags;
	}
}

static void try_set_prid_by_cpucfg(
	struct cpuinfo_loongarch_linux_processor *processors,
	uint32_t count)
{
	uint32_t prid;
	if (!cpucfg(CPUCFG_REG_PRID, &prid)) {
		return;
	}
	for (uint32_t i = 0; i < count; i++) {
		processors[i].prid = prid;
		processors[i].flags |= CPUINFO_LOONGARCH_LINUX_VALID_PRID;
	}
}

void cpuinfo_loongarch_linux_init(void) {
	struct cpuinfo_loongarch_linux_processor* loongarch_linux_processors = NULL;
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_package* packages = NULL;
	struct cpuinfo_uarch_info* uarchs = NULL;
	const struct cpuinfo_processor** linux_cpu_to_processor_map = NULL;
	const struct cpuinfo_core** linux_cpu_to_core_map = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;
	struct cpuinfo_cache* l3 = NULL;
	uint32_t* linux_cpu_to_uarch_index_map = NULL;

	const uint32_t max_processors_count = cpuinfo_linux_get_max_processors_count();
	cpuinfo_log_debug("system maximum processors count: %"PRIu32, max_processors_count);

	const uint32_t max_possible_processors_count = 1 +
		cpuinfo_linux_get_max_possible_processor(max_processors_count);
	cpuinfo_log_debug("maximum possible processors count: %"PRIu32, max_possible_processors_count);
	const uint32_t max_present_processors_count = 1 +
		cpuinfo_linux_get_max_present_processor(max_processors_count);
	cpuinfo_log_debug("maximum present processors count: %"PRIu32, max_present_processors_count);

	uint32_t valid_processor_mask = CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR;
	uint32_t loongarch_linux_processors_count = max_processors_count;
	if (max_present_processors_count != 0) {
		loongarch_linux_processors_count = min(loongarch_linux_processors_count, max_present_processors_count);
		valid_processor_mask = CPUINFO_LINUX_FLAG_PRESENT;
	}
	if (max_possible_processors_count != 0) {
		loongarch_linux_processors_count = min(loongarch_linux_processors_count, max_possible_processors_count);
		valid_processor_mask |= CPUINFO_LINUX_FLAG_POSSIBLE;
	}
	if ((max_present_processors_count | max_possible_processors_count) == 0) {
		cpuinfo_log_error("failed to parse both lists of possible and present processors");
		return;
	}

	loongarch_linux_processors = calloc(loongarch_linux_processors_count, sizeof(struct cpuinfo_loongarch_linux_processor));
	if (loongarch_linux_processors == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" Loongarch logical processors",
			loongarch_linux_processors_count * sizeof(struct cpuinfo_loongarch_linux_processor),
			loongarch_linux_processors_count);
		return;
	}

	if (max_possible_processors_count) {
		cpuinfo_linux_detect_possible_processors(
			loongarch_linux_processors_count, &loongarch_linux_processors->flags,
			sizeof(struct cpuinfo_loongarch_linux_processor),
			CPUINFO_LINUX_FLAG_POSSIBLE);
	}

	if (max_present_processors_count) {
		cpuinfo_linux_detect_present_processors(
			loongarch_linux_processors_count, &loongarch_linux_processors->flags,
			sizeof(struct cpuinfo_loongarch_linux_processor),
			CPUINFO_LINUX_FLAG_PRESENT);
	}

	#if CPUINFO_ARCH_LOONGARCH64
	/* Populate ISA structure with hwcap information. */
	cpuinfo_loongarch64_linux_decode_isa_from_hwcap(&cpuinfo_isa);
	if (cpuinfo_isa.cpucfg) {
		try_set_prid_by_cpucfg(loongarch_linux_processors, loongarch_linux_processors_count);
		try_set_cache_by_cpucfg(loongarch_linux_processors, loongarch_linux_processors_count);
	}
	#endif

	/* Populate processor information. */
	uint32_t valid_processors = 0;

	if (!cpuinfo_loongarch_linux_parse_proc_cpuinfo(
			loongarch_linux_processors_count,
			loongarch_linux_processors)) {
		cpuinfo_log_error("failed to parse processor information from /proc/cpuinfo");
		return;
	}

	if (!cpuinfo_loongarch_linux_parse_cpu_cache(
			loongarch_linux_processors_count,
			loongarch_linux_processors)) {
		cpuinfo_log_error("failed to parse processor information from /sys/devices/system/cpu/cpuX/cache/indexY/*");
		return;
	}

	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (bitmask_all(loongarch_linux_processors[i].flags, valid_processor_mask)) {
			valid_processors += 1;
			loongarch_linux_processors[i].system_processor_id = i;
			loongarch_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_VALID;
			continue;
		}
		if (!(loongarch_linux_processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR)) {
			/*
			 * Processor is in possible and present lists, but not reported in /proc/cpuinfo.
			 * This is fairly common: high-index processors can be not reported if they are offline.
			 */
			cpuinfo_log_info("processor %"PRIu32" is not listed in /proc/cpuinfo", i);
			continue;
		}
		/* Processor reported in /proc/cpuinfo, but not in possible and/or present lists: log and ignore */
		cpuinfo_log_warning("invalid processor %"PRIu32" reported in /proc/cpuinfo", i);
	}

	/* Populate core information. */
	uint32_t last_core_id = UINT32_MAX, core_count = 0, smt_id = UINT32_MAX;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID))
			continue;
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_CORE_ID)) {
			cpuinfo_log_warning("Not set core id for processor %"PRIu32" from /proc/cpuinfo", i);
			loongarch_linux_processors[i].core_id = last_core_id;
		}
		smt_id += 1;
		if (loongarch_linux_processors[i].core_id != last_core_id) {
			core_count += 1;
			smt_id = 0;
			last_core_id = loongarch_linux_processors[i].core_id;
		}
		loongarch_linux_processors[i].smt_id = smt_id;
		loongarch_linux_processors[i].flags |= CPUINFO_LINUX_FLAG_SMT_ID;
	}

	/* Not populate cluster information. Thought a package as a cluster. */
	uint32_t cluster_count;

	/* Populate package information. */
	uint32_t last_package_id = UINT32_MAX, package_count = 0, package_leader_id = UINT32_MAX;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID))
			continue;
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_PACKAGE_ID)) {
			cpuinfo_log_warning("Not set package id for processor %"PRIu32" from /proc/cpuinfo", i);
			loongarch_linux_processors[i].package_id = last_package_id;
			if (package_leader_id == UINT32_MAX) {
				cpuinfo_log_warning("Set default package leader id 0 for processor %"PRIu32, i);
				package_leader_id = 0;
			}
		}
		if (loongarch_linux_processors[i].package_id != last_package_id) {
			package_count += 1;
			last_package_id = loongarch_linux_processors[i].package_id;
			package_leader_id = i;
		}
		loongarch_linux_processors[i].package_leader_id = package_leader_id;
	}
	cluster_count = package_count;

	/* Initialize core vendor, uarch for every logical processor */
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID | CPUINFO_LINUX_FLAG_PACKAGE_ID))
			continue;
		const uint32_t package_leader = loongarch_linux_processors[i].package_leader_id;
		if (package_leader == i) {
			/* Package leader: decode core vendor and uarch */
			cpuinfo_loongarch_decode_vendor_uarch(
				loongarch_linux_processors[package_leader].prid,
				&loongarch_linux_processors[package_leader].vendor,
				&loongarch_linux_processors[package_leader].uarch);
		} else {
			/* Package non-leader: copy vendor, uarch from package leader */
			loongarch_linux_processors[i].vendor = loongarch_linux_processors[package_leader].vendor;
			loongarch_linux_processors[i].uarch = loongarch_linux_processors[package_leader].uarch;
		}
	}

	qsort(loongarch_linux_processors, loongarch_linux_processors_count,
		sizeof(struct cpuinfo_loongarch_linux_processor), cmp_loongarch_linux_processor);


	uint32_t uarchs_count = 0;
	enum cpuinfo_uarch last_uarch = cpuinfo_uarch_unknown;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID))
			continue;
		if (uarchs_count == 0 || loongarch_linux_processors[i].uarch != last_uarch) {
			last_uarch = loongarch_linux_processors[i].uarch;
			uarchs_count += 1;
		}
		loongarch_linux_processors[i].uarch_index = uarchs_count - 1;
	}

	processors = calloc(valid_processors, sizeof(struct cpuinfo_processor));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
			valid_processors * sizeof(struct cpuinfo_processor), valid_processors);
		goto cleanup;
	}

	cores = calloc(core_count, sizeof(struct cpuinfo_core));
	if (cores == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" cores",
			core_count * sizeof(struct cpuinfo_core), core_count);
		goto cleanup;
	}

	clusters = calloc(cluster_count, sizeof(struct cpuinfo_cluster));
	if (clusters == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" core clusters",
			cluster_count * sizeof(struct cpuinfo_cluster), cluster_count);
		goto cleanup;
	}

	packages = calloc(cluster_count, sizeof(struct cpuinfo_package));
	if (packages == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" core packages",
			cluster_count * sizeof(struct cpuinfo_package), package_count);
		goto cleanup;
	}

	uarchs = calloc(uarchs_count, sizeof(struct cpuinfo_uarch_info));
	if (uarchs == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" microarchitectures",
			uarchs_count * sizeof(struct cpuinfo_uarch_info), uarchs_count);
		goto cleanup;
	}

	linux_cpu_to_processor_map = calloc(loongarch_linux_processors_count, sizeof(struct cpuinfo_processor*));
	if (linux_cpu_to_processor_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" logical processor mapping entries",
			loongarch_linux_processors_count * sizeof(struct cpuinfo_processor*), loongarch_linux_processors_count);
		goto cleanup;
	}

	linux_cpu_to_core_map = calloc(loongarch_linux_processors_count, sizeof(struct cpuinfo_core*));
	if (linux_cpu_to_core_map == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" core mapping entries",
			loongarch_linux_processors_count * sizeof(struct cpuinfo_core*), loongarch_linux_processors_count);
		goto cleanup;
	}

	if (uarchs_count > 1) {
		linux_cpu_to_uarch_index_map = calloc(loongarch_linux_processors_count, sizeof(uint32_t));
		if (linux_cpu_to_uarch_index_map == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for %"PRIu32" uarch index mapping entries",
				loongarch_linux_processors_count * sizeof(uint32_t), loongarch_linux_processors_count);
			goto cleanup;
		}
	}

	l1i = calloc(valid_processors, sizeof(struct cpuinfo_cache));
	if (l1i == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
			valid_processors * sizeof(struct cpuinfo_cache), valid_processors);
		goto cleanup;
	}

	l1d = calloc(valid_processors, sizeof(struct cpuinfo_cache));
	if (l1d == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
			valid_processors * sizeof(struct cpuinfo_cache), valid_processors);
		goto cleanup;
	}

	// Victim Cache is private. TODO dynamic detect
	l2 = calloc(valid_processors, sizeof(struct cpuinfo_cache));
	if (l2 == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
			valid_processors * sizeof(struct cpuinfo_cache), valid_processors);
		goto cleanup;
	}

	// Shared in package.
	l3 = calloc(package_count, sizeof(struct cpuinfo_cache));
	if (l3 == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
			valid_processors * sizeof(struct cpuinfo_cache), valid_processors);
		goto cleanup;
	}

	uint32_t uarchs_index = 0;
	last_uarch = cpuinfo_uarch_unknown;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		if (!bitmask_all(loongarch_linux_processors[i].flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}
		if (uarchs_index == 0 || loongarch_linux_processors[i].uarch != last_uarch) {
			last_uarch = loongarch_linux_processors[i].uarch;
			uarchs[uarchs_index] = (struct cpuinfo_uarch_info) {
				.uarch = loongarch_linux_processors[i].uarch,
				.prid = loongarch_linux_processors[i].prid,
			};
			uarchs_index += 1;
		}
		uarchs[uarchs_index - 1].processor_count += 1;
		uarchs[uarchs_index - 1].core_count += loongarch_linux_processors[i].smt_id == 0 ? 1 : 0;
	}

	/* Transfer contents of processor list to ABI structures. */
	uint32_t processor_index = UINT32_MAX, core_index = UINT32_MAX, cluster_index = UINT32_MAX, package_index = UINT32_MAX;
	last_core_id = last_package_id = UINT32_MAX;
	for (uint32_t i = 0; i < loongarch_linux_processors_count; i++) {
		struct cpuinfo_loongarch_linux_processor *cur = &loongarch_linux_processors[i];

		if (!bitmask_all(cur->flags, CPUINFO_LINUX_FLAG_VALID)) {
			continue;
		}

		processor_index += 1;
		core_index += last_core_id != cur->core_id ? 1 : 0;
		cluster_index += last_package_id != cur->package_id ? 1 : 0;
		package_index += last_package_id != cur->package_id ? 1 : 0;
		processors[processor_index] = (struct cpuinfo_processor) {
			.smt_id = cur->smt_id,
			.core = &cores[core_index],
			.cluster = &clusters[cluster_index],
			.package = &packages[package_index],
			.linux_id = (int) cur->system_processor_id,
			.cache.l1i = &l1i[processor_index],
			.cache.l1d = &l1d[processor_index],
			.cache.l2 = &l2[processor_index],
			.cache.l3 = &l3[package_index],
		};

		memcpy(&l1i[processor_index], &cur->l1i, sizeof(struct cpuinfo_cache));
		memcpy(&l1d[processor_index], &cur->l1d, sizeof(struct cpuinfo_cache));
		memcpy(&l2[processor_index],  &cur->l2,  sizeof(struct cpuinfo_cache));
		l1i[processor_index].processor_start = processor_index;
		l1i[processor_index].processor_count = 1;
		l1d[processor_index].processor_start = processor_index;
		l1d[processor_index].processor_count = 1;
		l2[processor_index].processor_start = processor_index;
		l2[processor_index].processor_count = 1;

		if (cur->smt_id == 0) {
			cores[core_index] = (struct cpuinfo_core) {
				.processor_start = processor_index,
				.processor_count = 1,
				.core_id = cur->core_id,
				.cluster = &clusters[cluster_index],
				.package = &packages[package_index],
				.vendor = cur->vendor,
				.uarch = cur->uarch,
				.prid = cur->prid,
			};
			last_core_id = cur->core_id;
		} else {
			cores[core_index].processor_count += 1;
		}

		if (cur->package_leader_id == cur->system_processor_id) {
			clusters[cluster_index] = (struct cpuinfo_cluster) {
				.processor_start = processor_index,
				.processor_count = 1,
				.core_start = core_index,
				.core_count = 1,
				.cluster_id = 0, // Thought a package as a cluster
				.package = &packages[package_index],
				.vendor = cur->vendor,
				.uarch = cur->uarch,
				.prid = cur->prid,
			};
			packages[package_index] = (struct cpuinfo_package) {
				.processor_start = processor_index,
				.processor_count = 1,
				.core_start = core_index,
				.core_count = 1,
				.cluster_start = cluster_index,
				.cluster_count = 1,
			};
			memcpy(&l3[package_index],  &cur->l3,  sizeof(struct cpuinfo_cache));
			l3[package_index].processor_start = processor_index;
			l3[package_index].processor_count = 1;
			last_package_id = cur->package_id;
		} else {
			clusters[cluster_index].processor_count += 1;
			clusters[cluster_index].core_count += cur->smt_id == 0 ? 1 : 0;
			packages[package_index].processor_count += 1;
			packages[package_index].core_count += cur->smt_id == 0 ? 1 : 0;
			l3[package_index].processor_count += 1;
		}

		linux_cpu_to_processor_map[cur->system_processor_id] = &processors[processor_index];
		linux_cpu_to_core_map[cur->system_processor_id] = &cores[core_index];
		if (linux_cpu_to_uarch_index_map != NULL) {
			linux_cpu_to_uarch_index_map[cur->system_processor_id] = cur->uarch_index;
		}
	}

	/* Commit */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
	cpuinfo_clusters = clusters;
	cpuinfo_packages = packages;
	cpuinfo_uarchs = uarchs;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2;
	cpuinfo_cache[cpuinfo_cache_level_3]  = l3;

	cpuinfo_processors_count = valid_processors;
	cpuinfo_cores_count = core_count;
	cpuinfo_clusters_count = cluster_count;
	cpuinfo_packages_count = package_count;
	cpuinfo_uarchs_count = uarchs_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = valid_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = valid_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = valid_processors;
	cpuinfo_cache_count[cpuinfo_cache_level_3]  = package_count;
	cpuinfo_max_cache_size = cpuinfo_compute_max_cache_size(&processors[0]);

	cpuinfo_linux_cpu_max = loongarch_linux_processors_count;
	cpuinfo_linux_cpu_to_processor_map = linux_cpu_to_processor_map;
	cpuinfo_linux_cpu_to_core_map = linux_cpu_to_core_map;
	cpuinfo_linux_cpu_to_uarch_index_map = linux_cpu_to_uarch_index_map;

	__sync_synchronize();
	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;
	clusters = NULL;
	uarchs = NULL;
	l1i = l1d = l2 = l3 = NULL;
	linux_cpu_to_processor_map = NULL;
	linux_cpu_to_core_map = NULL;
	linux_cpu_to_uarch_index_map = NULL;

cleanup:
	free(loongarch_linux_processors);
	free(processors);
	free(cores);
	free(clusters);
	free(uarchs);
	free(l1i);
	free(l1d);
	free(l2);
	free(l3);
	free(linux_cpu_to_processor_map);
	free(linux_cpu_to_core_map);
	free(linux_cpu_to_uarch_index_map);
}
