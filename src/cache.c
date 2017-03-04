#include <cpuinfo.h>
#include <api.h>


struct cpuinfo_cache* cpuinfo_cache[cpuinfo_cache_level_max];
uint32_t cpuinfo_cache_count[cpuinfo_cache_level_max];

struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l1i_cache(void) {
	return (struct cpuinfo_caches) {
		.count = cpuinfo_cache_count[cpuinfo_cache_level_1i],
		.instances = cpuinfo_cache[cpuinfo_cache_level_1i]
	};
}

struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l1d_cache(void) {
	return (struct cpuinfo_caches) {
		.count = cpuinfo_cache_count[cpuinfo_cache_level_1d],
		.instances = cpuinfo_cache[cpuinfo_cache_level_1d]
	};
}

struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l2_cache(void) {
	return (struct cpuinfo_caches) {
		.count = cpuinfo_cache_count[cpuinfo_cache_level_2],
		.instances = cpuinfo_cache[cpuinfo_cache_level_2]
	};
}

struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l3_cache(void) {
	return (struct cpuinfo_caches) {
		.count = cpuinfo_cache_count[cpuinfo_cache_level_3],
		.instances = cpuinfo_cache[cpuinfo_cache_level_3]
	};
}

struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l4_cache(void) {
	return (struct cpuinfo_caches) {
		.count = cpuinfo_cache_count[cpuinfo_cache_level_4],
		.instances = cpuinfo_cache[cpuinfo_cache_level_4]
	};
}
