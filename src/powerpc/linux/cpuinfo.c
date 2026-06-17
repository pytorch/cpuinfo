#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo/log.h>
#include <linux/api.h>
#include <powerpc/linux/api.h>

/*
 * Size, in chars, of the on-stack buffer used for parsing lines of /proc/cpuinfo.
 * This is also the limit on the length of a single line.
 */
#define BUFFER_SIZE 1024

struct proc_cpuinfo_parser_state {
	uint32_t processor_index;
	uint32_t max_processors_count;
	struct cpuinfo_powerpc_linux_processor* processors;
	struct cpuinfo_powerpc_linux_processor dummy_processor;
};

static uint32_t parse_processor_number(const char* processor_start, const char* processor_end) {
	const size_t processor_length = (size_t)(processor_end - processor_start);

	if (processor_length == 0) {
		cpuinfo_log_warning("Processor number in /proc/cpuinfo is ignored: string is empty");
		return 0;
	}

	uint32_t processor_number = 0;
	for (const char* digit_ptr = processor_start; digit_ptr != processor_end; digit_ptr++) {
		const uint32_t digit = (uint32_t)(*digit_ptr - '0');
		if (digit >= 10) {
			cpuinfo_log_warning(
				"non-decimal suffix %.*s in /proc/cpuinfo processor number is ignored",
				(int)(processor_end - digit_ptr),
				digit_ptr);
			break;
		}

		processor_number = processor_number * 10 + digit;
	}

	return processor_number;
}

static void parse_cpu_architecture(
	const char* cpu_architecture_start,
	const char* cpu_architecture_end,
	struct cpuinfo_powerpc_linux_processor* processor) {
	const size_t cpu_arch_name_length = 5;

	if (!memcmp(cpu_architecture_start, "POWER", cpu_arch_name_length)) {
		processor->flags |= CPUINFO_POWERPC_LINUX_VALID_ARCHITECTURE;

		/* For now we are assuming the implementer is always valid.  */
		processor->flags |= CPUINFO_POWERPC_LINUX_VALID_IMPLEMENTER;
		/* For now we only support IBM vendor.  */
		processor->core.vendor = cpuinfo_vendor_ibm;

		const char* cpu_arch_ptr = cpu_architecture_start + cpu_arch_name_length;
		uint32_t arch_version = 0;
		for (; cpu_arch_ptr != cpu_architecture_end; cpu_arch_ptr++) {
			const uint32_t digit = (uint32_t)(*cpu_arch_ptr - '0');

			if (digit >= 10) {
				break;
			}
			arch_version = arch_version * 10 + digit;
		}

		switch (arch_version) {
			case 9: /* POWER9 */
				processor->core.uarch = cpuinfo_uarch_power9;
				break;
			case 10: /* POWER10 */
				processor->core.uarch = cpuinfo_uarch_power10;
				break;
			case 11: /* POWER11 */
				processor->core.uarch = cpuinfo_uarch_power11;
				break;
			default:
			unsupported:
				cpuinfo_log_error(
					"CPU architecture %.*s in /proc/cpuinfo is ignored due to a unsupported architecture version",
					(int)(cpu_architecture_end - cpu_architecture_start),
					cpu_architecture_start);
		}
		processor->flags |= CPUINFO_POWERPC_LINUX_VALID_PROCESSOR;
	} else {
		cpuinfo_log_warning(
			"processor %.*s in /proc/cpuinfo is ignored due to not a Power processor",
			(int)(cpu_architecture_end - cpu_architecture_start),
			cpu_architecture_start);
	}
}

static void parse_cpu_pvr(
	const char* cpu_revision_start,
	const char* cpu_revision_end,
	struct cpuinfo_powerpc_linux_processor* processor) {
	const char* cpu_rev_ptr = cpu_revision_start;
	uint16_t revision = 0;
	uint16_t version = 0;
	processor->core.pvr = 0x0;

	for (; cpu_rev_ptr != cpu_revision_end; cpu_rev_ptr++) {
		if (*cpu_rev_ptr == '(') {
			cpu_rev_ptr++; // Skip
			break;
		}
	}

	size_t pvr_str_len = 3;
	if (memcmp(cpu_rev_ptr, "pvr", pvr_str_len) == 0) {
		/* Parse revision. */
		uint16_t revision = 0;
		cpu_rev_ptr += pvr_str_len + 1; // Skip pvr string + space
		for (; cpu_rev_ptr != cpu_revision_end; cpu_rev_ptr++) {
			if (*cpu_rev_ptr == ' ') {
				cpu_rev_ptr++;
				break;
			}
			uint32_t digit = (uint32_t)(*cpu_rev_ptr - '0');
			if (digit >= 10) {
				digit = digit - 0x27;
				if ((digit < 10) || (digit > 15)) {
					cpuinfo_log_warning(
						"cpu revision %.*s in /proc/cpuinfo is ignored due non number",
						(int)(cpu_revision_end - cpu_revision_start),
						cpu_revision_start);
					return;
				}
			}
			revision = revision * 16 + digit;
		}

		/* Parse version. */
		uint16_t version = 0;
		for (; cpu_rev_ptr != cpu_revision_end; cpu_rev_ptr++) {
			if (*cpu_rev_ptr == ')') {
				cpu_rev_ptr++;
				break;
			}
			uint32_t digit = (uint32_t)(*cpu_rev_ptr - '0');
			if (digit >= 10) {
				digit = digit - 0x27;
				if ((digit < 10) || (digit > 15)) {
					cpuinfo_log_warning(
						"cpu version %.*s in /proc/cpuinfo is ignored due non number",
						(int)(cpu_revision_end - cpu_revision_start),
						cpu_revision_start);
					return;
				}
			}
			version = version * 16 + digit;
		}

		processor->core.pvr |= (revision << 16);
		processor->core.pvr |= version;
	} else {
		cpuinfo_log_warning(
			"cpu revision %.*s in /proc/cpuinfo is ignored due non PVR information",
			(int)(cpu_revision_end - cpu_revision_start),
			cpu_revision_start);
	}
}

static bool parse_line(
	const char* line_start,
	const char* line_end,
	struct proc_cpuinfo_parser_state state[restrict static 1],
	uint64_t line_number) {
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
		cpuinfo_log_warning(
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
		cpuinfo_log_warning(
			"Line %.*s in /proc/cpuinfo is ignored: key contains only spaces",
			(int)(line_end - line_start),
			line_start);
		return true;
	}

	/* Skip leading spaces in value part. */
	const char* value_start = separator + 1;
	for (; value_start != line_end; value_start++) {
		if (*value_start != ' ') {
			break;
		}
	}

	/* Value part contains nothing but spaces. Skip line. */
	if (value_start == line_end) {
		cpuinfo_log_warning(
			"Line %.*s in /proc/cpuinfo is ignored: value contains only spaces",
			(int)(line_end - line_start),
			line_start);
	}

	/* Skip trailing spaces in value part (if any) */
	const char* value_end = line_end;
	for (; value_end != value_start; value_end--) {
		if (value_end[-1] != ' ') {
			break;
		}
	}

	/* Declarations to return */
	const uint32_t processor_index = state->processor_index;
	const uint32_t max_processors_count = state->max_processors_count;
	struct cpuinfo_powerpc_linux_processor* processors = state->processors;
	struct cpuinfo_powerpc_linux_processor* processor = &state->dummy_processor;

	if (processor_index < max_processors_count) {
		processor = &processors[processor_index];
	}

	const size_t key_length = key_end - line_start;

	switch (key_length) {
		case 3:
			if (memcmp(line_start, "cpu", key_length) == 0) {
				parse_cpu_architecture(value_start, value_end, processor);
			} else {
				cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
			}
			break;
		case 5:
			if (memcmp(line_start, "clock", key_length) == 0) {
				parse_cpu_architecture(value_start, value_end, processor);
			} else {
				cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
			}
			break;
		case 7:
			if (memcmp(line_start, "machine", key_length) == 0) {
				parse_cpu_architecture(value_start, value_end, processor);
			} else {
				cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
			}
			break;
		case 8:
			if (memcmp(line_start, "revision", key_length) == 0) {
				parse_cpu_pvr(value_start, value_end, processor);
			} else {
				cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
			}
			break;
		case 9:
			if (memcmp(line_start, "processor", key_length) == 0) {
				state->processor_index = parse_processor_number(value_start, value_end);
				return true;
			} else {
				cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
			}
			break;
		default:
		unknown:
			cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
	}
	return true;
}

bool cpuinfo_powerpc_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_powerpc_linux_processor processors[restrict static max_processors_count]) {
	struct proc_cpuinfo_parser_state state = {
		.processor_index = 0,
		.max_processors_count = max_processors_count,
		.processors = processors,
	};
	return cpuinfo_linux_parse_multiline_file(
		"/proc/cpuinfo", BUFFER_SIZE, (cpuinfo_line_callback)parse_line, &state);
}
