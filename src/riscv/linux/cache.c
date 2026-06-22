#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cpuinfo.h>
#include <cpuinfo/log.h>
#include <linux/api.h>
#include <riscv/linux/api.h>

static bool uint32_parser(
	const char* filename,
	const char* text_start,
	const char* text_end,
	void* context) {
	uint32_t* value_ptr = (uint32_t*)context;
	if (text_start == text_end) {
		return false;
	}
	uint32_t value = 0;
	const char* p = text_start;
	for (; p < text_end && *p >= '0' && *p <= '9'; p++) {
		value = value * 10 + (*p - '0');
	}
	if (p == text_start) {
		return false;
	}
	*value_ptr = value;
	return true;
}

static bool cache_size_parser(
	const char* filename,
	const char* text_start,
	const char* text_end,
	void* context) {
	uint32_t* size_ptr = (uint32_t*)context;
	if (text_start == text_end) {
		return false;
	}
	uint32_t value = 0;
	const char* p = text_start;
	while (p < text_end && *p >= '0' && *p <= '9') {
		value = value * 10 + (*p - '0');
		p++;
	}
	if (p == text_start || value == 0) {
		return false;
	}
	uint32_t multiplier = 1024;
	if (p < text_end) {
		const char c = (char)toupper((unsigned char)*p);
		if (c == 'M') {
			multiplier = 1024 * 1024;
		} else if (c == 'G') {
			multiplier = 1024 * 1024 * 1024;
		}
	}
	*size_ptr = value * multiplier;
	return true;
}

struct cache_type_context {
	bool is_instruction;
	bool is_data;
	bool is_unified;
};

static bool cache_type_parser(
	const char* filename,
	const char* text_start,
	const char* text_end,
	void* context) {
	struct cache_type_context* type = (struct cache_type_context*)context;
	if (text_end - text_start >= 11 && memcmp(text_start, "Instruction", 11) == 0) {
		type->is_instruction = true;
	} else if (text_end - text_start >= 7 && memcmp(text_start, "Unified", 7) == 0) {
		type->is_unified = true;
	} else if (text_end - text_start >= 4 && memcmp(text_start, "Data", 4) == 0) {
		type->is_data = true;
	}
	return true;
}

/*
 * Find the sysfs cache index for a given cpu, level, and type.
 * Type must be "Instruction", "Data", or "Unified" (exact match).
 * Returns the sysfs index, or UINT32_MAX if not found.
 */
static uint32_t find_cache_index(
	uint32_t cpu_id,
	uint32_t level,
	const char* type) {
	char path[256];
	for (uint32_t idx = 0; idx < 8; idx++) {
		snprintf(
			path,
			sizeof(path),
			"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/level",
			cpu_id,
			idx);
		uint32_t actual_level = 0;
		if (!cpuinfo_linux_parse_small_file(path, 16, uint32_parser, &actual_level)) {
			break;
		}
		if (actual_level != level) {
			continue;
		}

		snprintf(
			path,
			sizeof(path),
			"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/type",
			cpu_id,
			idx);
		struct cache_type_context ctx = {0};
		if (!cpuinfo_linux_parse_small_file(path, 32, cache_type_parser, &ctx)) {
			continue;
		}

		if ((strcmp(type, "Instruction") == 0 && ctx.is_instruction) ||
		    (strcmp(type, "Data") == 0 && ctx.is_data) ||
		    (strcmp(type, "Unified") == 0 && ctx.is_unified)) {
			return idx;
		}
	}
	return UINT32_MAX;
}

/*
 * Read cache properties from sysfs for a given cpu and cache index.
 * Returns true if at least the size was successfully read.
 */
static bool read_cache_properties(
	uint32_t cpu_id,
	uint32_t cache_index,
	struct cpuinfo_cache cache[restrict static 1]) {
	char path[256];

	uint32_t size = 0;
	snprintf(
		path,
		sizeof(path),
		"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/size",
		cpu_id,
		cache_index);
	if (!cpuinfo_linux_parse_small_file(path, 32, cache_size_parser, &size) || size == 0) {
		return false;
	}
	cache->size = size;

	uint32_t line_size = 0;
	snprintf(
		path,
		sizeof(path),
		"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/coherency_line_size",
		cpu_id,
		cache_index);
	cpuinfo_linux_parse_small_file(path, 16, uint32_parser, &line_size);
	cache->line_size = line_size;

	uint32_t sets = 0;
	snprintf(
		path,
		sizeof(path),
		"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/number_of_sets",
		cpu_id,
		cache_index);
	cpuinfo_linux_parse_small_file(path, 16, uint32_parser, &sets);
	cache->sets = sets;

	uint32_t associativity = 0;
	snprintf(
		path,
		sizeof(path),
		"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/ways_of_associativity",
		cpu_id,
		cache_index);
	cpuinfo_linux_parse_small_file(path, 16, uint32_parser, &associativity);
	cache->associativity = associativity;

	cache->partitions = 1;
	return true;
}

static bool sharing_info_callback(
	uint32_t range_start,
	uint32_t range_end,
	void* context) {
	struct cpuinfo_riscv_cache_sharing_info* info =
		(struct cpuinfo_riscv_cache_sharing_info*)context;
	if (range_start < info->min_cpu) {
		info->min_cpu = range_start;
	}
	info->cpu_count += range_end - range_start;
	return true;
}

bool cpuinfo_riscv_linux_parse_cache_from_sysfs(
	uint32_t cpu_id,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1]) {
	bool found_any = false;
	uint32_t idx;

	/* L1 Instruction cache. */
	idx = find_cache_index(cpu_id, 1, "Instruction");
	if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l1i)) {
		found_any = true;
	}

	/* L1 Data cache; fall back to Unified if no Data type exists. */
	idx = find_cache_index(cpu_id, 1, "Data");
	if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l1d)) {
		found_any = true;
	} else {
		idx = find_cache_index(cpu_id, 1, "Unified");
		if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l1d)) {
			l1d->flags = CPUINFO_CACHE_UNIFIED;
			found_any = true;
		}
	}

	/* L2 cache: try Unified first, then Data. */
	idx = find_cache_index(cpu_id, 2, "Unified");
	if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l2)) {
		l2->flags = CPUINFO_CACHE_UNIFIED;
		found_any = true;
	} else {
		idx = find_cache_index(cpu_id, 2, "Data");
		if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l2)) {
			found_any = true;
		}
	}

	/* L3 cache: try Unified first, then Data. */
	idx = find_cache_index(cpu_id, 3, "Unified");
	if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l3)) {
		l3->flags = CPUINFO_CACHE_UNIFIED;
		found_any = true;
	} else {
		idx = find_cache_index(cpu_id, 3, "Data");
		if (idx != UINT32_MAX && read_cache_properties(cpu_id, idx, l3)) {
			found_any = true;
		}
	}

	return found_any;
}

bool cpuinfo_riscv_linux_get_cache_sharing(
	uint32_t cpu_id,
	uint32_t cache_level,
	struct cpuinfo_riscv_cache_sharing_info sharing[restrict static 1]) {
	sharing->min_cpu = cpu_id;
	sharing->cpu_count = 1;

	/* Find any cache index at this level. */
	uint32_t idx = find_cache_index(cpu_id, cache_level, "Unified");
	if (idx == UINT32_MAX) {
		idx = find_cache_index(cpu_id, cache_level, "Data");
	}
	if (idx == UINT32_MAX) {
		idx = find_cache_index(cpu_id, cache_level, "Instruction");
	}
	if (idx == UINT32_MAX) {
		return false;
	}

	char path[256];
	snprintf(
		path,
		sizeof(path),
		"/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/shared_cpu_list",
		cpu_id,
		idx);

	struct cpuinfo_riscv_cache_sharing_info info = {
		.min_cpu = UINT32_MAX,
		.cpu_count = 0,
	};
	if (cpuinfo_linux_parse_cpulist(path, sharing_info_callback, &info) &&
	    info.cpu_count > 0) {
		sharing->min_cpu = info.min_cpu;
		sharing->cpu_count = info.cpu_count;
	}
	return true;
}
