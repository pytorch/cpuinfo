#include <stddef.h>

#include <cpuinfo.h>
#include <api.h>


struct cpuinfo_processor* cpuinfo_processors = NULL;
struct cpuinfo_core* cpuinfo_cores = NULL;
struct cpuinfo_package* cpuinfo_packages = NULL;
struct cpuinfo_cache* cpuinfo_cache[cpuinfo_cache_level_max] = { NULL };

uint32_t cpuinfo_processors_count = 0;
uint32_t cpuinfo_cores_count = 0;
uint32_t cpuinfo_packages_count = 0;
uint32_t cpuinfo_cache_count[cpuinfo_cache_level_max] = { 0 };


const struct cpuinfo_processor* cpuinfo_get_processors(void) {
	return cpuinfo_processors;
}

const struct cpuinfo_core* cpuinfo_get_cores(void) {
	return cpuinfo_cores;
}

const struct cpuinfo_package* cpuinfo_get_packages(void) {
	return cpuinfo_packages;
}

const struct cpuinfo_processor* cpuinfo_get_processor(uint32_t index) {
	if (index < cpuinfo_processors_count) {
		return cpuinfo_processors + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_core* cpuinfo_get_core(uint32_t index) {
	if (index < cpuinfo_cores_count) {
		return cpuinfo_cores + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_package* cpuinfo_get_package(uint32_t index) {
	if (index < cpuinfo_packages_count) {
		return cpuinfo_packages + index;
	} else {
		return NULL;
	}
}

uint32_t cpuinfo_get_processors_count(void) {
	return cpuinfo_processors_count;
}

uint32_t cpuinfo_get_cores_count(void) {
	return cpuinfo_cores_count;
}

uint32_t cpuinfo_get_packages_count(void) {
	return cpuinfo_packages_count;
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1i_caches(void) {
	return cpuinfo_cache[cpuinfo_cache_level_1i];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1d_caches(void) {
	return cpuinfo_cache[cpuinfo_cache_level_1d];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l2_caches(void) {
	return cpuinfo_cache[cpuinfo_cache_level_2];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l3_caches(void) {
	return cpuinfo_cache[cpuinfo_cache_level_3];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l4_caches(void) {
	return cpuinfo_cache[cpuinfo_cache_level_4];
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1i_cache(uint32_t index) {
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_1i]) {
		return cpuinfo_cache[cpuinfo_cache_level_1i] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l1d_cache(uint32_t index) {
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_1d]) {
		return cpuinfo_cache[cpuinfo_cache_level_1d] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l2_cache(uint32_t index) {
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_2]) {
		return cpuinfo_cache[cpuinfo_cache_level_2] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l3_cache(uint32_t index) {
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_3]) {
		return cpuinfo_cache[cpuinfo_cache_level_3] + index;
	} else {
		return NULL;
	}
}

const struct cpuinfo_cache* CPUINFO_ABI cpuinfo_get_l4_cache(uint32_t index) {
	if (index < cpuinfo_cache_count[cpuinfo_cache_level_4]) {
		return cpuinfo_cache[cpuinfo_cache_level_4] + index;
	} else {
		return NULL;
	}
}

uint32_t CPUINFO_ABI cpuinfo_get_l1i_caches_count(void) {
	return cpuinfo_cache_count[cpuinfo_cache_level_1i];
}

uint32_t CPUINFO_ABI cpuinfo_get_l1d_caches_count(void) {
	return cpuinfo_cache_count[cpuinfo_cache_level_1d];
}

uint32_t CPUINFO_ABI cpuinfo_get_l2_caches_count(void) {
	return cpuinfo_cache_count[cpuinfo_cache_level_2];
}

uint32_t CPUINFO_ABI cpuinfo_get_l3_caches_count(void) {
	return cpuinfo_cache_count[cpuinfo_cache_level_3];
}

uint32_t CPUINFO_ABI cpuinfo_get_l4_caches_count(void) {
	return cpuinfo_cache_count[cpuinfo_cache_level_4];
}
