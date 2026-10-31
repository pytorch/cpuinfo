#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <cpuinfo/log.h>
#include <linux/api.h>
#include <powerpc/linux/api.h>

/*
 * Size, in chars, of the on-stack buffer used for parsing lines of
 * /proc/cpuinfo. This is also the limit on the length of a single line.
 */
#define BUFFER_SIZE 1024

struct proc_cpuinfo_parser_state {
	uint32_t processor_index;
	uint32_t max_processors_count;
	struct cpuinfo_powerpc_linux_processor* processors;
	struct cpuinfo_powerpc_linux_processor dummy_processor;
};

static bool key_equals(const char* key_start, const char* key_end, const char* key) {
	const size_t key_length = strlen(key);
	return (size_t)(key_end - key_start) == key_length && memcmp(key_start, key, key_length) == 0;
}

/*
 * Parses the processor number, e.g. "processor : 12".
 */
static bool parse_processor_number(
	const char* value_start,
	const char* value_end,
	uint32_t processor_number[restrict static 1]) {
	uint32_t value = 0;
	for (const char* digit_ptr = value_start; digit_ptr != value_end; digit_ptr++) {
		const uint32_t digit = (uint32_t)(*digit_ptr - '0');
		if (digit >= 10) {
			return false;
		}
		value = value * 10 + digit;
	}
	*processor_number = value;
	return true;
}

/*
 * Parses the name of the processor, e.g. "cpu : POWER9, altivec supported" or
 * "cpu : POWER10 (architected), altivec supported", into "POWER9" or
 * "POWER10".
 */
static void parse_cpu(
	const char* value_start,
	const char* value_end,
	struct cpuinfo_powerpc_linux_processor processor[restrict static 1]) {
	const char* name_end = value_start;
	for (; name_end != value_end; name_end++) {
		if (*name_end == ',' || *name_end == ' ') {
			break;
		}
	}

	size_t name_length = (size_t)(name_end - value_start);
	if (name_length >= CPUINFO_PACKAGE_NAME_MAX) {
		cpuinfo_log_debug(
			"processor name \"%.*s\" in /proc/cpuinfo is truncated", (int)name_length, value_start);
		name_length = CPUINFO_PACKAGE_NAME_MAX - 1;
	}
	memcpy(processor->name, value_start, name_length);
	processor->name[name_length] = '\0';
}

/*
 * Parses the Processor Version Register from the revision, e.g.
 * "revision : 2.3 (pvr 004e 1203)".
 */
static void parse_revision(
	const char* value_start,
	const char* value_end,
	struct cpuinfo_powerpc_linux_processor processor[restrict static 1]) {
	static const char pvr_prefix[] = "(pvr ";
	const size_t pvr_prefix_length = sizeof(pvr_prefix) - 1;

	const char* pvr_start = NULL;
	for (const char* prefix_ptr = value_start; (size_t)(value_end - prefix_ptr) >= pvr_prefix_length;
	     prefix_ptr++) {
		if (memcmp(prefix_ptr, pvr_prefix, pvr_prefix_length) == 0) {
			pvr_start = prefix_ptr + pvr_prefix_length;
			break;
		}
	}
	if (pvr_start == NULL) {
		cpuinfo_log_debug(
			"revision \"%.*s\" in /proc/cpuinfo has no PVR", (int)(value_end - value_start), value_start);
		return;
	}

	uint32_t pvr = 0;
	uint32_t pvr_digits = 0;
	for (const char* digit_ptr = pvr_start; digit_ptr != value_end && *digit_ptr != ')'; digit_ptr++) {
		const char c = *digit_ptr;
		uint32_t digit;
		if (c >= '0' && c <= '9') {
			digit = (uint32_t)(c - '0');
		} else if (c >= 'a' && c <= 'f') {
			digit = (uint32_t)(c - 'a' + 10);
		} else if (c >= 'A' && c <= 'F') {
			digit = (uint32_t)(c - 'A' + 10);
		} else if (c == ' ') {
			continue;
		} else {
			pvr_digits = 0;
			break;
		}
		pvr = (pvr << 4) | digit;
		pvr_digits++;
	}
	if (pvr_digits != 8) {
		cpuinfo_log_warning(
			"failed to parse PVR in revision \"%.*s\" in /proc/cpuinfo",
			(int)(value_end - value_start),
			value_start);
		return;
	}

	processor->pvr = pvr;
	processor->flags |= CPUINFO_LINUX_FLAG_PROC_CPUINFO;
}

static bool parse_line(const char* line_start, const char* line_end, void* context, uint64_t line_number) {
	struct proc_cpuinfo_parser_state* restrict state = context;
	/* Empty line. Skip. */
	if (line_start == line_end) {
		return true;
	}

	/* Search for ':' on the line. */
	const char* separator = line_start;
	for (; separator != line_end; separator++) {
		if (*separator == ':') {
			break;
		}
	}
	/* Skip line if no ':' separator was found. */
	if (separator == line_end) {
		cpuinfo_log_debug(
			"Line %.*s in /proc/cpuinfo is ignored: key/value separator ':' not found",
			(int)(line_end - line_start),
			line_start);
		return true;
	}

	/* Skip trailing spaces in key part. */
	const char* key_end = separator;
	for (; key_end != line_start; key_end--) {
		if (key_end[-1] != ' ' && key_end[-1] != '\t') {
			break;
		}
	}
	/* Skip line if key contains nothing but spaces. */
	if (key_end == line_start) {
		cpuinfo_log_debug(
			"Line %.*s in /proc/cpuinfo is ignored: key contains only spaces",
			(int)(line_end - line_start),
			line_start);
		return true;
	}

	/* Skip leading spaces in value part. */
	const char* value_start = separator + 1;
	for (; value_start != line_end; value_start++) {
		if (*value_start != ' ' && *value_start != '\t') {
			break;
		}
	}
	/* Value part contains nothing but spaces. Skip line. */
	if (value_start == line_end) {
		cpuinfo_log_debug(
			"Line %.*s in /proc/cpuinfo is ignored: value contains only spaces",
			(int)(line_end - line_start),
			line_start);
		return true;
	}

	/* Skip trailing spaces in value part (if any) */
	const char* value_end = line_end;
	for (; value_end != value_start; value_end--) {
		if (value_end[-1] != ' ') {
			break;
		}
	}

	if (key_equals(line_start, key_end, "processor")) {
		uint32_t processor_number = 0;
		if (!parse_processor_number(value_start, value_end, &processor_number)) {
			cpuinfo_log_warning(
				"processor number \"%.*s\" in /proc/cpuinfo is not an unsigned number",
				(int)(value_end - value_start),
				value_start);
			/* Send the following lines to the dummy processor. */
			processor_number = UINT32_MAX;
		}
		state->processor_index = processor_number;
		return true;
	}

	struct cpuinfo_powerpc_linux_processor* processor = &state->dummy_processor;
	if (state->processor_index < state->max_processors_count) {
		processor = &state->processors[state->processor_index];
	}

	if (key_equals(line_start, key_end, "cpu")) {
		parse_cpu(value_start, value_end, processor);
	} else if (key_equals(line_start, key_end, "revision")) {
		parse_revision(value_start, value_end, processor);
	}
	return true;
}

bool cpuinfo_powerpc_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_powerpc_linux_processor processors[restrict static 1]) {
	struct proc_cpuinfo_parser_state state = {
		/* Lines before the first "processor" line describe no processor. */
		.processor_index = UINT32_MAX,
		.max_processors_count = max_processors_count,
		.processors = processors,
	};
	return cpuinfo_linux_parse_multiline_file("/proc/cpuinfo", BUFFER_SIZE, parse_line, &state);
}
