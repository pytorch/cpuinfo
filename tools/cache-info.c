#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <cpuinfo.h>


void report_cache(
	uint32_t count, const struct cpuinfo_cache* cache,
	uint32_t level, const char* nonunified_type)
{
	const char* type = (cache->flags & CPUINFO_CACHE_UNIFIED) ? "unified" : nonunified_type;
	printf("L%"PRIu32" %s cache: ", level, type);

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
		printf("%"PRIu32" x ", count);
	}
	if (level == 1) {
		printf("%"PRIu32" %s, ", size, units);
	} else {
		printf("%"PRIu32" %s (%s), ", size, units, (cache->flags & CPUINFO_CACHE_INCLUSIVE) ? "inclusive" : "exclusive");
	}

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
	if (cache->processor_count != 0) {
		printf(", shared by %"PRIu32" processors\n", cache->processor_count);
	} else {
		printf("\n");
	}
}

void report_distinct_caches(
	uint32_t count, const struct cpuinfo_cache *cache,
	uint32_t level, const char *label)
{
	uint32_t similar_count = 0;
	uint32_t prev = 0;
	for (uint32_t i=0; i<count; ++i) {
		if (i != prev &&
			(cache[i].size != cache[prev].size
			|| cache[i].associativity != cache[prev].associativity
			|| cache[i].sets != cache[prev].sets
			|| cache[i].partitions != cache[prev].partitions
			|| cache[i].line_size != cache[prev].line_size
			|| cache[i].flags != cache[prev].flags
			|| cache[i].processor_count != cache[prev].processor_count))
		{
			similar_count = i - prev;
			report_cache(similar_count, &cache[prev], level, label);
			prev = i;
		}
	}

	similar_count = count - prev;
	if (similar_count > 0) {
		report_cache(similar_count, &cache[prev], level, label);
	}
}

void debug_print_caches(const char *label, const struct cpuinfo_cache * const cache, uint32_t count)
{
	for (uint32_t i=0; i<count; ++i) {
		printf("%s cache[%"PRIu32"]: processors [%"PRIu32"-%"PRIu32"]\n",
				   label, i, cache[i].processor_start,
				   (cache[i].processor_start + cache[i].processor_count - 1));
		printf("\tsize: %"PRIu32"\n", cache[i].size);
		printf("\tassociativity: %"PRIu32"\n", cache[i].associativity);
		printf("\tsets: %"PRIu32"\n", cache[i].sets);
		printf("\tpartitions: %"PRIu32"\n", cache[i].partitions);
		printf("\tline_size: %"PRIu32"\n", cache[i].line_size);
		printf("\tflags: %"PRIu32"\n", cache[i].flags);
	}
}

int main(int argc, char** argv) {
	if (!cpuinfo_initialize()) {
		fprintf(stderr, "failed to initialize CPU information\n");
		exit(EXIT_FAILURE);
	}

	if (argc > 1 && 0 == strcmp(argv[1], "-verbose")) {
		debug_print_caches("L1I", cpuinfo_get_l1i_caches(), cpuinfo_get_l1i_caches_count());
		debug_print_caches("L1D", cpuinfo_get_l1d_caches(), cpuinfo_get_l1d_caches_count());
		debug_print_caches("L2", cpuinfo_get_l2_caches(), cpuinfo_get_l2_caches_count());
		debug_print_caches("L3", cpuinfo_get_l3_caches(), cpuinfo_get_l3_caches_count());
		debug_print_caches("L4", cpuinfo_get_l4_caches(), cpuinfo_get_l4_caches_count());
	}

	printf("Max cache size (upper bound): %"PRIu32" bytes\n", cpuinfo_get_max_cache_size());

	if (cpuinfo_get_l1i_caches_count() != 0 && (cpuinfo_get_l1i_cache(0)->flags & CPUINFO_CACHE_UNIFIED) == 0) {
		report_distinct_caches(cpuinfo_get_l1i_caches_count(), cpuinfo_get_l1i_caches(), 1, "instruction");
	}
	if (cpuinfo_get_l1d_caches_count() != 0) {
		report_distinct_caches(cpuinfo_get_l1d_caches_count(), cpuinfo_get_l1d_caches(), 1, "data");
	}
	if (cpuinfo_get_l2_caches_count() != 0) {
		report_distinct_caches(cpuinfo_get_l2_caches_count(), cpuinfo_get_l2_caches(), 2, "data");
	}
	if (cpuinfo_get_l3_caches_count() != 0) {
		report_distinct_caches(cpuinfo_get_l3_caches_count(), cpuinfo_get_l3_caches(), 3, "data");
	}
	if (cpuinfo_get_l4_caches_count() != 0) {
		report_distinct_caches(cpuinfo_get_l4_caches_count(), cpuinfo_get_l4_caches(), 4, "data");
	}
}
