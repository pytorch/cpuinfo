#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <cpuinfo/log.h>
#include <linux/api.h>
#include <riscv/linux/api.h>

/*
 * Size, in chars, of the on-stack buffer used for parsing lines of /proc/cpuinfo.
 * This is also the limit on the length of a single line.
 */
#define BUFFER_SIZE 2048

static uint32_t parse_processor_number(
		const char* processor_start,
		const char* processor_end)
{
	const size_t processor_length = (size_t) (processor_end - processor_start);

	if (processor_length == 0) {
		cpuinfo_log_warning("Processor number in /proc/cpuinfo is ignored: string is empty");
		return 0;
	}

	uint32_t processor_number = 0;
	for (const char* digit_ptr = processor_start; digit_ptr != processor_end; digit_ptr++) {
		const uint32_t digit = (uint32_t) (*digit_ptr - '0');
		if (digit > 10) {
			cpuinfo_log_warning("non-decimal suffix %.*s in /proc/cpuinfo processor number is ignored",
								(int) (processor_end - digit_ptr), digit_ptr);
			break;
		}

		processor_number = processor_number * 10 + digit;
	}

	return processor_number;
}

static void parse_isa(
		const char* isa_start,
		const char* isa_end,
		struct cpuinfo_riscv_linux_processor processor[restrict static 1])
{
	const size_t isa_length = (size_t) (isa_end - isa_start);
	if (!(memcmp(isa_start, "rv32", 4) == 0 || memcmp(isa_start, "rv64", 4) == 0)) {
		cpuinfo_log_error("Invalid isa format in /proc/cpuinfo: %.*s. It should start with either `rv32` or `rv64`",
						  (int) (isa_length), isa_start);
		return;
	}

	/* Mark the features as valid */
	processor->flags |= CPUINFO_RISCV_LINUX_VALID_FEATURES | CPUINFO_RISCV_LINUX_VALID_PROCESSOR;

	const char* feature = isa_start + 4;
	for (;feature != isa_end; feature++) {
		if(*feature == 'i') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_I;
		} else if(*feature == 'm') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_M;
		} else if(*feature == 'a') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_A;
		} else if(*feature == 'f') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_F;
		} else if(*feature == 'd') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_D;
		} else if(*feature == 'c') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_C;
		} else if(*feature == 'v') {
			processor->features |= CPUINFO_RISCV_LINUX_FEATURE_V;
		} else {
			cpuinfo_log_warning("unexpected /proc/cpuinfo feature \"%.*s\" is ignored", 1, feature);
		}
	}
}

static void parse_uarch(
		const char* uarch_start,
		const char* uarch_end,
		struct cpuinfo_riscv_linux_processor processor[restrict static 1])
{
	/* Empty line. Skip. */
	if (uarch_start == uarch_end) {
		return;
	}

	/* Search for ',' on the line. */
	const char* separator = uarch_start;
	for (; separator != uarch_end; separator++) {
		if (*separator == ',') {
			break;
		}
	}
	/* Skip line if no ',' separator was found. */
	if (separator == uarch_end) {
		cpuinfo_log_info("Line %.*s in uarch is ignored: vendor/model separator ',' not found",
						 (int) (uarch_end - uarch_start), uarch_start);
		return;
	}

	const char* vendor_end = separator;
	/* Skip vendor if key contains nothing but spaces. */
	if (vendor_end == uarch_start) {
		cpuinfo_log_info("Line %.*s in uarch is ignored: vendor contains only spaces",
						 (int) (vendor_end - uarch_start), uarch_start);
	} else {
		const size_t vendor_length = vendor_end - uarch_start;
		switch (vendor_length) {
			case 6:
				if (memcmp(uarch_start, "sifive", vendor_length) == 0) {
					processor->core.vendor = cpuinfo_vendor_sifive;
					processor->flags |= CPUINFO_RISCV_LINUX_VALID_IMPLEMENTER | CPUINFO_RISCV_LINUX_VALID_PROCESSOR;
				} else {
					goto unknown_vendor;
				}
				break;
			default:
			unknown_vendor:
				cpuinfo_log_debug("unknown vendor key: %.*s", (int) vendor_length, uarch_start);
		}
	}

	const char* model_start = separator + 1;
	/* Skip model if key contains nothing but spaces. */
	if (model_start == uarch_end) {
		cpuinfo_log_info("Line %.*s in uarch is ignored: model contains only spaces",
						 (int) (uarch_end - uarch_start), uarch_start);
		return;
	} else {
		const size_t model_length = uarch_end - model_start;
		switch (model_length) {
			case 6:
				if (memcmp(model_start, "u74-mc", model_length) == 0) {
					processor->core.uarch = cpuinfo_uarch_u74_mc;
					processor->flags |= CPUINFO_RISCV_LINUX_VALID_ARCHITECTURE | CPUINFO_RISCV_LINUX_VALID_PROCESSOR;
				} else {
					goto unknown_model;
				}
				break;
			default:
			unknown_model:
				cpuinfo_log_debug("unknown model key: %.*s", (int) model_length, model_start);
		}
	}
}

struct proc_cpuinfo_parser_state {
	uint32_t processor_index;
	uint32_t max_processors_count;
	struct cpuinfo_riscv_linux_processor* processors;
	struct cpuinfo_riscv_linux_processor dummy_processor;
};


/*
 *	Decode a single line of /proc/cpuinfo information.
 *	Lines have format <words-with-spaces>[ ]*:[ ]<space-separated words>
 */
static bool parse_line(
		const char* line_start,
		const char* line_end,
		struct proc_cpuinfo_parser_state state[restrict static 1],
		uint64_t line_number)
{
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
		cpuinfo_log_info("Line %.*s in /proc/cpuinfo is ignored: key/value separator ':' not found",
						 (int) (line_end - line_start), line_start);
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
		cpuinfo_log_info("Line %.*s in /proc/cpuinfo is ignored: key contains only spaces",
						 (int) (line_end - line_start), line_start);
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
		cpuinfo_log_info("Line %.*s in /proc/cpuinfo is ignored: value contains only spaces",
						 (int) (line_end - line_start), line_start);
		return true;
	}

	/* Skip trailing spaces in value part (if any) */
	const char* value_end = line_end;
	for (; value_end != value_start; value_end--) {
		if (value_end[-1] != ' ') {
			break;
		}
	}

	const uint32_t processor_index      = state->processor_index;
	const uint32_t max_processors_count = state->max_processors_count;
	struct cpuinfo_riscv_linux_processor* processors = state->processors;
	struct cpuinfo_riscv_linux_processor* processor  = &state->dummy_processor;
	if (processor_index < max_processors_count) {
		processor = &processors[processor_index];
	}

	const size_t key_length = key_end - line_start;
	switch (key_length) {
		case 3:
			if (memcmp(line_start, "mmu", key_length) == 0) {
				// Do nothing
			} else if (memcmp(line_start, "isa", key_length) == 0) {
				parse_isa(value_start, value_end, processor);
			} else {
				goto unknown;
			}
			break;
		case 4:
			if (memcmp(line_start, "hart", key_length) == 0) {
				// Do nothing
			} else {
				goto unknown;
			}
			break;
		case 5:
			if (memcmp(line_start, "uarch", key_length) == 0) {
				parse_uarch(value_start, value_end, processor);
			} else {
				goto unknown;
			}
			break;
		case 9:
			if (memcmp(line_start, "processor", key_length) == 0) {
				const uint32_t new_processor_index = parse_processor_number(value_start, value_end);
				if (new_processor_index < processor_index) {
					/* Strange: decreasing processor number */
					cpuinfo_log_warning(
						"unexpectedly low processor number %"PRIu32" following processor %"PRIu32" in /proc/cpuinfo",
						new_processor_index, processor_index);
				} else if (new_processor_index > processor_index + 1) {
					/* Strange, but common: skipped processor $(processor_index + 1) */
					cpuinfo_log_info(
						"unexpectedly high processor number %"PRIu32" following processor %"PRIu32" in /proc/cpuinfo",
						new_processor_index, processor_index);
				}
				if (new_processor_index >= max_processors_count) {
					/* Log and ignore processor */
					cpuinfo_log_warning("processor %"PRIu32" in /proc/cpuinfo is ignored: index exceeds system limit %"PRIu32,
										new_processor_index, max_processors_count - 1);
				} else {
					processors[new_processor_index].flags |= CPUINFO_LINUX_FLAG_PROC_CPUINFO;
				}
				state->processor_index = new_processor_index;
				return true;
			} else{
				goto unknown;
			}
			break;
		default:
		unknown:
			cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int) key_length, line_start);
	}
	return true;
}

bool cpuinfo_riscv_linux_parse_proc_cpuinfo(
		uint32_t max_processors_count,
		struct cpuinfo_riscv_linux_processor processors[restrict static max_processors_count])
{
	struct proc_cpuinfo_parser_state state = {
			.processor_index = 0,
			.max_processors_count = max_processors_count,
			.processors = processors,
	};
	return cpuinfo_linux_parse_multiline_file("/proc/cpuinfo", BUFFER_SIZE,
											  (cpuinfo_line_callback) parse_line, &state);
}