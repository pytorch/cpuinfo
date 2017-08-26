#include <stdio.h>
#include <inttypes.h>

#include <cpuinfo.h>


void report_cache(
	uint32_t count, const struct cpuinfo_cache cache[restrict static 1],
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

int main(int argc, char** argv) {
	cpuinfo_initialize();
	if ((cpuinfo_get_l1i_cache().count != 0 && cpuinfo_get_l1i_cache().instances->flags & CPUINFO_CACHE_UNIFIED) == 0) {
		report_cache(cpuinfo_get_l1i_cache().count, cpuinfo_get_l1i_cache().instances, 1, "instruction");
	}
	if (cpuinfo_get_l1d_cache().count != 0) {
		report_cache(cpuinfo_get_l1d_cache().count, cpuinfo_get_l1d_cache().instances, 1, "data");
	}
	if (cpuinfo_get_l2_cache().count != 0) {
		report_cache(cpuinfo_get_l2_cache().count, cpuinfo_get_l2_cache().instances, 2, "data");
	}
	if (cpuinfo_get_l3_cache().count != 0) {
		report_cache(cpuinfo_get_l3_cache().count, cpuinfo_get_l3_cache().instances, 3, "data");
	}
	if (cpuinfo_get_l4_cache().count != 0) {
		report_cache(cpuinfo_get_l4_cache().count, cpuinfo_get_l4_cache().instances, 4, "data");
	}
}
