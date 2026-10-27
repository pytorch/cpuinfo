#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <cpuinfo.h>

void report_cache(uint32_t count, const struct cpuinfo_cache* cache, uint32_t level, const char* nonunified_type) {
	const char* type = (cache->flags & CPUINFO_CACHE_UNIFIED) ? "unified" : nonunified_type;
	printf("L%" PRIu32 " %s cache: ", level, type);

	uint32_t size = cache->size;
	const char* units = "bytes";
	if (size % UINT32_C(1048576) == 0) {
		size /= UINT32_C(1048576);
		units = "MB";
	} else if (size % UINT32_C(1024) == 0) {
		size /= UINT32_C(1024);
		units = "KB";
	}
	if (count != 1) {
		printf("%" PRIu32 " x ", count);
	}
	if (level == 1) {
		printf("%" PRIu32 " %s, ", size, units);
	} else {
		printf("%" PRIu32 " %s (%s), ",
		       size,
		       units,
		       (cache->flags & CPUINFO_CACHE_INCLUSIVE) ? "inclusive" : "exclusive");
	}

	if (cache->associativity * cache->line_size == cache->size) {
		printf("fully associative");
	} else {
		printf("%" PRIu32 "-way set associative", cache->associativity);
	}
	if (cache->sets != 0) {
		printf(" (%" PRIu32 " sets", cache->sets);
		if (cache->partitions != 1) {
			printf(", %" PRIu32 " partitions", cache->partitions);
		}
		if (cache->flags & CPUINFO_CACHE_COMPLEX_INDEXING) {
			printf(", complex indexing), ");
		} else {
			printf("), ");
		}
	}

	printf("%" PRIu32 " byte lines", cache->line_size);
	if (cache->processor_count != 0) {
		printf(", shared by %" PRIu32 " processors\n", cache->processor_count);
	} else {
		printf("\n");
	}
}

#include <stdbool.h>

static bool are_caches_equal(const struct cpuinfo_cache* a, const struct cpuinfo_cache* b) {
	return a->size == b->size && a->associativity == b->associativity && a->sets == b->sets &&
		a->partitions == b->partitions && a->line_size == b->line_size && a->flags == b->flags &&
		a->processor_count == b->processor_count;
}

static void report_cache_group(
	uint32_t count,
	const struct cpuinfo_cache* (*get_cache)(uint32_t),
	uint32_t level,
	const char* nonunified_type) {
	uint32_t i = 0;
	while (i < count) {
		const struct cpuinfo_cache* cache = get_cache(i);
		uint32_t group_count = 1;
		while (i + group_count < count && are_caches_equal(cache, get_cache(i + group_count))) {
			group_count++;
		}
		report_cache(group_count, cache, level, nonunified_type);
		i += group_count;
	}
}

int main(int argc, char** argv) {
	if (!cpuinfo_initialize()) {
		fprintf(stderr, "failed to initialize CPU information\n");
		exit(EXIT_FAILURE);
	}
	printf("Max cache size (upper bound): %" PRIu32 " bytes\n", cpuinfo_get_max_cache_size());

	if (cpuinfo_get_l1i_caches_count() != 0 && (cpuinfo_get_l1i_cache(0)->flags & CPUINFO_CACHE_UNIFIED) == 0) {
		report_cache_group(cpuinfo_get_l1i_caches_count(), cpuinfo_get_l1i_cache, 1, "instruction");
	}
	if (cpuinfo_get_l1d_caches_count() != 0) {
		report_cache_group(cpuinfo_get_l1d_caches_count(), cpuinfo_get_l1d_cache, 1, "data");
	}
	if (cpuinfo_get_l2_caches_count() != 0) {
		report_cache_group(cpuinfo_get_l2_caches_count(), cpuinfo_get_l2_cache, 2, "data");
	}
	if (cpuinfo_get_l3_caches_count() != 0) {
		report_cache_group(cpuinfo_get_l3_caches_count(), cpuinfo_get_l3_cache, 3, "data");
	}
	if (cpuinfo_get_l4_caches_count() != 0) {
		report_cache_group(cpuinfo_get_l4_caches_count(), cpuinfo_get_l4_cache, 4, "data");
	}
}
