#include <stddef.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>


bool cpuinfo_is_initialized = false;

struct cpuinfo_processor* cpuinfo_processors = NULL;
struct cpuinfo_core* cpuinfo_cores = NULL;
struct cpuinfo_cluster* cpuinfo_clusters = NULL;
struct cpuinfo_package* cpuinfo_packages = NULL;
struct cpuinfo_cache* cpuinfo_cache[cpuinfo_cache_level_max] = { NULL };

uint32_t cpuinfo_processors_count = 0;
uint32_t cpuinfo_cores_count = 0;
uint32_t cpuinfo_clusters_count = 0;
uint32_t cpuinfo_packages_count = 0;
uint32_t cpuinfo_cache_count[cpuinfo_cache_level_max] = { 0 };


const struct cpuinfo_processor* cpuinfo_get_processors(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "processors");
	}
	return cpuinfo_processors;
}

const struct cpuinfo_core* cpuinfo_get_cores(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "core");
	}
	return cpuinfo_cores;
}

const struct cpuinfo_cluster* cpuinfo_get_clusters(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "clusters");
	}
	return cpuinfo_clusters;
}

const struct cpuinfo_package* cpuinfo_get_packages(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "packages");
	}
	return cpuinfo_packages;
}

const struct cpuinfo_processor* cpuinfo_get_processor(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "processor");
	}
	if (index < cpuinfo_processors_count) {
		return cpuinfo_processors + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_core* cpuinfo_get_core(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "core");
	}
	if (index < cpuinfo_cores_count) {
		return cpuinfo_cores + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cluster* cpuinfo_get_cluster(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "cluster");
	}
	if (index < cpuinfo_clusters_count) {
		return cpuinfo_clusters + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_package* cpuinfo_get_package(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "package");
	}
	if (index < cpuinfo_packages_count) {
		return cpuinfo_packages + index;
	} else {
		return NULL;
	}
}

uint32_t cpuinfo_get_processors_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "processors_count");
	}
	return cpuinfo_processors_count;
}

uint32_t cpuinfo_get_cores_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "cores_count");
	}
	return cpuinfo_cores_count;
}

uint32_t cpuinfo_get_clusters_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "clusters_count");
	}
	return cpuinfo_clusters_count;
}

uint32_t cpuinfo_get_packages_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "packages_count");
	}
	return cpuinfo_packages_count;
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1i_caches(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l1i_caches");
	}
	return cpuinfo_cache[cpuinfo_cache_level_1i];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1d_caches(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l1d_caches");
	}
	return cpuinfo_cache[cpuinfo_cache_level_1d];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l2_caches(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l2_caches");
	}
	return cpuinfo_cache[cpuinfo_cache_level_2];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l3_caches(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l3_caches");
	}
	return cpuinfo_cache[cpuinfo_cache_level_3];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l4_caches(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l4_caches");
	}
	return cpuinfo_cache[cpuinfo_cache_level_4];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1i_cache(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l1i_cache");
	}
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_1i]) {
		return cpuinfo_cache[cpuinfo_cache_level_1i] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1d_cache(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l1d_cache");
	}
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_1d]) {
		return cpuinfo_cache[cpuinfo_cache_level_1d] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l2_cache(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l2_cache");
	}
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_2]) {
		return cpuinfo_cache[cpuinfo_cache_level_2] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l3_cache(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l3_cache");
	}
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_3]) {
		return cpuinfo_cache[cpuinfo_cache_level_3] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l4_cache(uint32_t index) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l4_cache");
	}
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_4]) {
		return cpuinfo_cache[cpuinfo_cache_level_4] + index;
	} else {
		return NULL;
	}
}

uint32_t CPUINFO_ABI cpuinfo_get_l1i_caches_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l1i_caches_count");
	}
	return cpuinfo_cache_count[cpuinfo_cache_level_1i];
}

uint32_t CPUINFO_ABI cpuinfo_get_l1d_caches_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l1d_caches_count");
	}
	return cpuinfo_cache_count[cpuinfo_cache_level_1d];
}

uint32_t CPUINFO_ABI cpuinfo_get_l2_caches_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l2_caches_count");
	}
	return cpuinfo_cache_count[cpuinfo_cache_level_2];
}

uint32_t CPUINFO_ABI cpuinfo_get_l3_caches_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l3_caches_count");
	}
	return cpuinfo_cache_count[cpuinfo_cache_level_3];
}

uint32_t CPUINFO_ABI cpuinfo_get_l4_caches_count(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "l4_caches_count");
	}
	return cpuinfo_cache_count[cpuinfo_cache_level_4];
}
