#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <cpuinfo.h>
#include <cpuinfo/log.h>
#include <powerpc/api.h>
#include <sys/auxv.h>

#define NUM_CACHE   4
#define BUF_SIZE    128

int path_exist(const char *path)
{
	return (access(path, F_OK) == 0);
}

void read_str(char *result, size_t len, const char *path)
{
	FILE *f;

	f = fopen(path, "r");

	fgets(result, len, f);
	fclose(f);

	len = strlen(result);
	if (result[len - 1] == '\n')
		result[len - 1] = '\0';
}

void decode_cache_features(uint32_t cache_features[], unsigned long geometry)
{
	cache_features[1] = (geometry >> 16) & 0xffff;
	cache_features[2] = geometry & 0xffff;
	/* If associativity = 65535 it means upper limit of 16 bit of AT_L%d_CACHEGEOMETRY has reached so we calculate it from, size, sets and line_size*/
	if(cache_features[1] == 65535)
		cache_features[1] = cache_features[0]/(cache_features[2]*cache_features[3]);
}

void cpuinfo_powerpc_decode_cache(
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1])
{
	char buf[BUF_SIZE];
	char result[BUF_SIZE];
	uint32_t cache_features[NUM_CACHE];
	uint32_t i;
	uint32_t size = 0, sets = 0;
	size_t len;

	for(i = 0; i < NUM_CACHE; i++) {

		sprintf (buf, "/sys/devices/system/cpu/cpu%d/cache/index%d/number_of_sets", i);
		if(path_exist(buf))
		read_str(result, sizeof(result), buf);

		sets = atoi(result);
		cache_features[3] = sets;

		unsigned long geometry;

		switch (i) {
			case 0:
				geometry = getauxval(AT_L1D_CACHEGEOMETRY);
				size = getauxval(AT_L1D_CACHESIZE);
				cache_features[0] = size;
				decode_cache_features(cache_features, geometry);
				sets = size/(cache_features[1] * cache_features[2]);
				*l1d = (struct cpuinfo_cache) {
					.size = size,
					.associativity = cache_features[1],
					.line_size = cache_features[2],
					.sets = sets,
					.partitions = 1

				};
				break;
			case 1:
				geometry = getauxval(AT_L1I_CACHEGEOMETRY);
				size = getauxval(AT_L1I_CACHESIZE);
				cache_features[0] = size;
				decode_cache_features(cache_features, geometry);
				sets = size/(cache_features[1] * cache_features[2]);
				*l1i = (struct cpuinfo_cache) {
					.size = size,
					.associativity = cache_features[1],
					.line_size = cache_features[2],
					.sets = sets,
					.partitions = 1
				};
				break;
			case 2:
				geometry = getauxval(AT_L2_CACHEGEOMETRY);
				size = getauxval(AT_L2_CACHESIZE);
				cache_features[0] = size;
				decode_cache_features(cache_features, geometry);
				sets = size/(cache_features[1] * cache_features[2]);
				*l2 = (struct cpuinfo_cache) {
					.size = size,
					.associativity = cache_features[1],
					.line_size = cache_features[2],
					.sets = sets,
					.partitions = 1
				};
				break;
			case 3:
				geometry = getauxval(AT_L3_CACHEGEOMETRY);
				size = getauxval(AT_L3_CACHESIZE);
				cache_features[0] = size;
				decode_cache_features(cache_features, geometry);
				sets = size/(cache_features[1] * cache_features[2]);
				*l3 = (struct cpuinfo_cache) {
					.size = size,
					.associativity = cache_features[1],
					.line_size = cache_features[2],
					.sets = sets,
					.partitions = 1
				};
				break;

		}
	}
}
