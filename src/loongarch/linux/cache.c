#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <loongarch/linux/api.h>
#include <cpuinfo/log.h>
#include <linux/api.h>

#define BUFFER_SIZE 64
#define STRINGIFY(token) #token
#define CACHE_INDEX_SIZE \
	(sizeof("/sys/devices/system/cpu/cpu" STRINGIFY(UINT32_MAX) "/cache/index" STRINGIFY(UINT32_MAX) "/") + 32)
#define CACHE_INDEX_FORMAT "/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/%s"
#define FILENAME_SETS STRINGIFY(number_of_sets)
#define FILENAME_WAYS STRINGIFY(ways_of_associativity)
#define FILENAME_LSIZE STRINGIFY(coherency_line_size)
#define FILENAME_LEVEL STRINGIFY(level)
#define FILENAME_TYPE STRINGIFY(type)
#define FILENAME_SIZE STRINGIFY(size)

enum cache_type {
	Unknown,
	Instruction,
	Data,
	Unified,
};

static bool parse_number(
	const char* line_start,
	const char* line_end,
	uint32_t number_ptr[restrict static 1],
	uint64_t line_number)
{
	uint32_t number = 0;
	const char* parsed = line_start;
	if (line_number != 1) {
		return true;
	}
	for (; parsed != line_end && *parsed != '\0'; parsed++) {
		const uint32_t digit = (uint32_t)(uint8_t)(*parsed) - (uint32_t)'0';
		if (digit >= 10) {
			return false;
		}
		number = number * UINT32_C(10) + digit;
	}
	*number_ptr = number;
	return true;
}

static bool parse_size(
	const char* line_start,
	const char* line_end,
	uint32_t number_ptr[restrict static 1],
	uint64_t line_number)
{
	uint32_t number = 0;
	const char* parsed = line_start;
	if (line_number != 1) {
		return true;
	}
	for (; parsed != line_end && *parsed != 'K'; parsed++) {
		const uint32_t digit = (uint32_t)(uint8_t)(*parsed) - (uint32_t)'0';
		if (digit >= 10) {
			return false;
		}
		number = number * UINT32_C(10) + digit;
	}
	if (*parsed != 'K') {
		return false;
	}
	*number_ptr = number;
	return true;
}

static inline size_t min(size_t a, size_t b) {
	return a < b ? a : b;
}

static bool parse_type(
	const char* line_start,
	const char* line_end,
	enum cache_type type_ptr[restrict static 1],
	uint64_t line_number)
{
	size_t line_length = line_end - line_start;

	if (line_number != 1) {
		return true;
	}
	if (0 == strncmp("Instruction", line_start, min(line_length, sizeof("Instruction")))) {
		*type_ptr = Instruction;
		return true;
	}
	if (0 == strncmp("Data", line_start, min(line_length, sizeof("Data")))) {
		*type_ptr = Data;
		return true;
	}
	if (0 == strncmp("Unified", line_start, min(line_length, sizeof("Unified")))) {
		*type_ptr = Unified;
		return true;
	}
	return false;
}

static bool parse(
	const char* suffix, uint32_t x, uint32_t y, void *data_ptr,
	cpuinfo_line_callback parse_func)
{
	char filename[CACHE_INDEX_SIZE];
	const int chars_formatted =
		snprintf(filename, CACHE_INDEX_SIZE, CACHE_INDEX_FORMAT, x, y, suffix);
	if ((unsigned int)chars_formatted >= CACHE_INDEX_SIZE) {
		cpuinfo_log_warning("failed to format filename for cache index %s", suffix);
		return true;
	}
	if (!cpuinfo_linux_parse_multiline_file(filename, BUFFER_SIZE, parse_func, data_ptr)) {
		cpuinfo_log_error("failed parse cache index %s", suffix);
		return false;
	}
	return true;
}

static bool cpuinfo_loongarch_linux_parse_cpuX_cache_indexY(
	uint32_t x, uint32_t y,
	struct cpuinfo_loongarch_linux_processor processors[restrict static 1])
{
	uint32_t sets, ways, level, line_size, size;
	enum cache_type type = Unknown;
	char filename[CACHE_INDEX_SIZE];
	struct cpuinfo_cache *cache;

	if (!parse(FILENAME_SETS, x, y, &sets, (cpuinfo_line_callback) parse_number)) {
		return false;
	}
	if (!parse(FILENAME_WAYS, x, y, &ways, (cpuinfo_line_callback) parse_number)) {
		return false;
	}
	if (!parse(FILENAME_LSIZE, x, y, &line_size, (cpuinfo_line_callback) parse_number)) {
		return false;
	}
	if (!parse(FILENAME_LEVEL, x, y, &level, (cpuinfo_line_callback) parse_number)) {
		return false;
	}
	if (!parse(FILENAME_TYPE, x, y, &type, (cpuinfo_line_callback) parse_type)) {
		return false;
	}
	if (!parse(FILENAME_SIZE, x, y, &size, (cpuinfo_line_callback) parse_size)) {
		return false;
	}

	if (level == 1 && type == Instruction) {
		cache = &processors->l1i;
		processors->flags |= CPUINFO_LOONGARCH_LINUX_VALID_L1I;
	} else if (level == 1 && type == Data) {
		cache = &processors->l1d;
		processors->flags |= CPUINFO_LOONGARCH_LINUX_VALID_L1D;
	} else if (level == 2 && type == Unified) {
		cache = &processors->l2;
		processors->flags |= CPUINFO_LOONGARCH_LINUX_VALID_L2;
	} else if (level == 3 && type == Unified) {
		cache = &processors->l3;
		processors->flags |= CPUINFO_LOONGARCH_LINUX_VALID_L3;
	} else {
		return false;
	}

	*cache = (struct cpuinfo_cache) {
		.associativity = ways,
		.sets = sets,
		.line_size = line_size,
		.size = size * 1024,
		.partitions = 1,
	};
	return true;
}

bool cpuinfo_loongarch_linux_parse_cpu_cache(
	uint32_t max_processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors_count])
{

	for (uint32_t i = 0; i < max_processors_count; i++) {
		// TODO dynamic detect
		for (uint32_t j = 0; j < 4; j++) {
			if ((processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_L1D) && j == 0) {
				continue;
			}
			if ((processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_L1I) && j == 1) {
				continue;
			}
			if ((processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_L2) && j == 2) {
				continue;
			}
			if ((processors[i].flags & CPUINFO_LOONGARCH_LINUX_VALID_L3) && j == 3) {
				continue;
			}
			if (!cpuinfo_loongarch_linux_parse_cpuX_cache_indexY(i, j, &processors[i]))
				return false;
		}
	}
	return true;
}
