#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cpuinfo/log.h>
#include <linux/api.h>
#include <powerpc/linux/api.h>

#define STRINGIFY(token) #token

/* Highest cache index in sysfs to look for. */
#define MAX_CACHE_INDEX 8

#define CACHE_FILENAME_SIZE                                                                                     \
	(sizeof("/sys/devices/system/cpu/cpu" STRINGIFY(UINT32_MAX) "/cache/index" STRINGIFY(UINT32_MAX) "/") + \
	 sizeof("ways_of_associativity"))
#define CACHE_FILENAME_FORMAT "/sys/devices/system/cpu/cpu%" PRIu32 "/cache/index%" PRIu32 "/%s"

/* Size of the buffer for the small sysfs files. */
#define CACHE_FILESIZE 64

static bool is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* Copies a small sysfs file, without trailing whitespace, into a string. */
struct string_context {
	char* string;
	size_t size;
};

static bool string_parser(const char* filename, const char* text_start, const char* text_end, void* context) {
	struct string_context* string_context = context;
	while (text_end != text_start && is_whitespace(text_end[-1])) {
		text_end--;
	}
	const size_t length = (size_t)(text_end - text_start);
	if (length >= string_context->size) {
		cpuinfo_log_warning("failed to parse file %s: value is too long", filename);
		return false;
	}
	memcpy(string_context->string, text_start, length);
	string_context->string[length] = '\0';
	return true;
}

/*
 * Parses a small sysfs file holding an unsigned number, optionally followed by
 * a K or M suffix as in the "size" file.
 */
static bool uint32_parser(const char* filename, const char* text_start, const char* text_end, void* context) {
	uint32_t value = 0;
	const char* char_ptr = text_start;
	for (; char_ptr != text_end; char_ptr++) {
		const uint32_t digit = (uint32_t)(*char_ptr - '0');
		if (digit >= 10) {
			break;
		}
		value = value * 10 + digit;
	}
	if (char_ptr == text_start) {
		cpuinfo_log_warning(
			"failed to parse file %s: \"%.*s\" is not an unsigned number",
			filename,
			(int)(text_end - text_start),
			text_start);
		return false;
	}
	if (char_ptr != text_end && *char_ptr == 'K') {
		value *= UINT32_C(1024);
	} else if (char_ptr != text_end && *char_ptr == 'M') {
		value *= UINT32_C(1048576);
	}
	*((uint32_t*)context) = value;
	return true;
}

static bool read_cache_file(
	uint32_t processor,
	uint32_t index,
	const char* name,
	cpuinfo_smallfile_callback callback,
	void* context) {
	char filename[CACHE_FILENAME_SIZE];
	const int chars_formatted =
		snprintf(filename, CACHE_FILENAME_SIZE, CACHE_FILENAME_FORMAT, processor, index, name);
	if ((unsigned int)chars_formatted >= CACHE_FILENAME_SIZE) {
		cpuinfo_log_warning(
			"failed to format filename for %s of cache %" PRIu32 " of processor %" PRIu32,
			name,
			index,
			processor);
		return false;
	}
	return cpuinfo_linux_parse_small_file(filename, CACHE_FILESIZE, callback, context);
}

struct shared_cpus_context {
	uint32_t max_processors_count;
	const struct cpuinfo_powerpc_linux_processor* processors;
	uint32_t leader_id;
	uint32_t processor_count;
};

static bool shared_cpus_parser(uint32_t shared_cpus_start, uint32_t shared_cpus_end, void* context) {
	struct shared_cpus_context* shared_cpus_context = context;
	if (shared_cpus_end > shared_cpus_context->max_processors_count) {
		shared_cpus_end = shared_cpus_context->max_processors_count;
	}
	for (uint32_t shared_cpu = shared_cpus_start; shared_cpu < shared_cpus_end; shared_cpu++) {
		const uint32_t flags = shared_cpus_context->processors[shared_cpu].flags;
		if ((flags & CPUINFO_LINUX_FLAG_VALID) == 0) {
			continue;
		}
		if (shared_cpu < shared_cpus_context->leader_id) {
			shared_cpus_context->leader_id = shared_cpu;
		}
		shared_cpus_context->processor_count++;
	}
	return true;
}

void cpuinfo_powerpc_linux_detect_caches(
	uint32_t max_processors_count,
	uint32_t processor,
	struct cpuinfo_powerpc_linux_processor processors[restrict static 1]) {
	for (uint32_t index = 0; index < MAX_CACHE_INDEX; index++) {
		uint32_t level = 0;
		if (!read_cache_file(processor, index, "level", uint32_parser, &level)) {
			/* The cache indices are consecutive. */
			break;
		}

		char type[16];
		struct string_context type_context = {.string = type, .size = sizeof(type)};
		if (!read_cache_file(processor, index, "type", string_parser, &type_context)) {
			continue;
		}

		enum cpuinfo_powerpc_cache_level cache_level;
		if (level == 1 && strcmp(type, "Instruction") == 0) {
			cache_level = cpuinfo_powerpc_cache_level_l1i;
		} else if (level == 1 && strcmp(type, "Data") == 0) {
			cache_level = cpuinfo_powerpc_cache_level_l1d;
		} else if (level == 2) {
			cache_level = cpuinfo_powerpc_cache_level_l2;
		} else if (level == 3) {
			cache_level = cpuinfo_powerpc_cache_level_l3;
		} else {
			cpuinfo_log_debug(
				"ignoring L%" PRIu32 " %s cache %" PRIu32 " of processor %" PRIu32,
				level,
				type,
				index,
				processor);
			continue;
		}

		struct cpuinfo_powerpc_linux_cache cache = {0};
		if (!read_cache_file(processor, index, "size", uint32_parser, &cache.size) || cache.size == 0) {
			continue;
		}

		/* Not reported by every kernel and hypervisor. */
		read_cache_file(processor, index, "ways_of_associativity", uint32_parser, &cache.associativity);
		read_cache_file(processor, index, "coherency_line_size", uint32_parser, &cache.line_size);

		struct shared_cpus_context shared_cpus_context = {
			.max_processors_count = max_processors_count,
			.processors = processors,
			.leader_id = UINT32_MAX,
		};
		char shared_cpu_list_filename[CACHE_FILENAME_SIZE];
		snprintf(
			shared_cpu_list_filename,
			CACHE_FILENAME_SIZE,
			CACHE_FILENAME_FORMAT,
			processor,
			index,
			"shared_cpu_list");
		if (!cpuinfo_linux_parse_cpulist(shared_cpu_list_filename, shared_cpus_parser, &shared_cpus_context) ||
		    shared_cpus_context.processor_count == 0) {
			/* Assume that the cache is private to this processor. */
			shared_cpus_context.leader_id = processor;
			shared_cpus_context.processor_count = 1;
		}
		cache.leader_id = shared_cpus_context.leader_id;
		cache.processor_count = shared_cpus_context.processor_count;

		processors[processor].caches[cache_level] = cache;
	}
}
