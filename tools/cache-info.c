#include <stdio.h>
#include <inttypes.h>

#include <cpuinfo.h>


void report_cache(const struct cpuinfo_cache cache[restrict static 1], uint32_t level, const char* nonunified_type) {
	const char* type = (cache->flags & CPUINFO_CACHE_UNIFIED) ? "unified" : nonunified_type;
	printf("L%"PRIu32" %s cache: ", level, type);

	uint32_t count = cache->size;
	const char* units = "bytes";
	if (count % UINT32_C(1048576) == 0) {
		count /= UINT32_C(1048576);
		units = "MB";
	} else if (count % UINT32_C(1024) == 0) {
		count /= UINT32_C(1024);
		units = "KB";
	}
	printf("%"PRIu32" %s (%s), ", count, units, (cache->flags & CPUINFO_CACHE_INCLUSIVE) ? "inclusive" : "exclusive");

	if (cache->associativity * cache->line_size == cache->size) {
		printf("fully associative");
	} else {
		printf("%"PRIu32"-way set associative", cache->associativity);
	}
	if (cache->sets != 0) {
		printf(" (%"PRIu32" sets", cache->sets);
		if (cache->partitions != 1) {
			printf(", %"PRIu32" partitions", cache->partitions);
		}
		if (cache->flags & CPUINFO_CACHE_COMPLEX_INDEXING) {
			printf(", complex indexing), ");
		} else {
			printf("), ");
		}
	}

	printf("%"PRIu32" byte lines", cache->line_size);
	if (cache->thread_count != 0) {
		printf(", shared by %"PRIu32" threads\n", cache->thread_count);
	} else {
		printf("\n");
	}
}

int main(int argc, char** argv) {
	cpuinfo_initialize();
	if ((cpuinfo_processors->cache.l1i->flags & CPUINFO_CACHE_UNIFIED) == 0) {
		report_cache(cpuinfo_processors->cache.l1i, 1, "instruction");
	}
	report_cache(cpuinfo_processors->cache.l1d, 1, "data");
	if (cpuinfo_processors->cache.l2 != NULL) {
		report_cache(cpuinfo_processors->cache.l2, 2, "data");
	}
	if (cpuinfo_processors->cache.l3 != NULL) {
		report_cache(cpuinfo_processors->cache.l3, 3, "data");
	}
	if (cpuinfo_processors->cache.l4 != NULL) {
		report_cache(cpuinfo_processors->cache.l4, 4, "data");
	}
}
