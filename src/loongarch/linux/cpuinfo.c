#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <linux/api.h>
#include <loongarch/linux/api.h>
#include <loongarch/prid.h>
#include <cpuinfo/log.h>

/*
 * Size, in chars, of the on-stack buffer used for parsing lines of /proc/cpuinfo.
 * This is also the limit on the length of a single line.
 */
#define BUFFER_SIZE 1024

struct cpuinfo_loongarch_seriesID {
	const char* prefix;
	enum prid_series seriesID;
};

static const struct cpuinfo_loongarch_seriesID loongson_name_map_seriesID[] = {
	{ .prefix = "3A5000", .seriesID = prid_series_la464, },
	{ .prefix = "3C5000", .seriesID = prid_series_la464, },
	{ .prefix = "3D5000", .seriesID = prid_series_la464, },
	{ .prefix = "3A6000", .seriesID = prid_series_la664, },
	{ .prefix = "3C6000", .seriesID = prid_series_la664, },
};

static inline size_t min(size_t a, size_t b) {
        return a < b ? a : b;
}

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

/*
 *	Full list of Loongarch features reported in /proc/cpuinfo:
 */
static void parse_features(
	const char* features_start,
	const char* features_end,
	struct cpuinfo_loongarch_linux_processor processor[restrict static 1])
{
	const char* feature_start = features_start;
	const char* feature_end;

	/* Mark the features as valid */
	processor->flags |= CPUINFO_LOONGARCH_LINUX_VALID_FEATURES;

	do {
		feature_end = feature_start + 1;
		for (; feature_end != features_end; feature_end++) {
			if (*feature_end == ' ') {
				break;
			}
		}
		const size_t feature_length = (size_t) (feature_end - feature_start);

		switch (feature_length) {
			case 3:
				if (memcmp(feature_start, "lam", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LAM;
					#endif
				} else if (memcmp(feature_start, "ual", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_UAL;
					#endif
				} else if (memcmp(feature_start, "lsx", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LSX;
					#endif
				} else if (memcmp(feature_start, "fpu", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_FPU;
					#endif
				} else if (memcmp(feature_start, "lvz", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LVZ;
					#endif
				} else if (memcmp(feature_start, "ptw", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_PTW;
					#endif
				} else {
					goto unexpected;
				}
				break;
			case 4:
				if (memcmp(feature_start, "lasx", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LASX;
					#endif
				} else if (memcmp(feature_start, "lspw", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LSPW;
					#endif
				} else {
					goto unexpected;
				}
				break;
			case 5:
				if (memcmp(feature_start, "crc32", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_CRC32;
					#endif
				} else {
					goto unexpected;
				}
 				break;
			case 6:
				if (memcmp(feature_start, "crypto", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_CRYPTO;
					#endif
				} else if (memcmp(feature_start, "cpucfg", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_CPUCFG;
					#endif
				} else {
					goto unexpected;
				}
				break;
			case 7:
				if (memcmp(feature_start, "complex", feature_length) == 0) {
					processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_COMPLEX;
				} else if (memcmp(feature_start, "lbt_x86", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_X86;
					#endif
				} else if (memcmp(feature_start, "lbt_arm", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_ARM;
					#endif
				} else {
					goto unexpected;
				}
				break;
			case 8:
				if (memcmp(feature_start, "lbt_mips", feature_length) == 0) {
					#if CPUINFO_ARCH_LOONGARCH64
						processor->features |= CPUINFO_LOONGARCH_LINUX_FEATURE_LBT_MIPS;
					#endif
				} else {
					goto unexpected;
				}
				break;
			default:
			unexpected:
				cpuinfo_log_warning("unexpected /proc/cpuinfo feature \"%.*s\" is ignored",
					(int) feature_length, feature_start);
				break;
		}
		feature_start = feature_end;
		for (; feature_start != features_end; feature_start++) {
			if (*feature_start != ' ') {
				break;
			}
		}
	} while (feature_start != feature_end);
}

static bool parse_loongson(const char* name_start, size_t length){
	/* expected loongson , its length is eight */
	if(length != 8) return false;
	/* expected loongson , its first char is 'l' or 'L' */
	if(name_start[0] != 'l' && name_start[0] != 'L') return false;
	
	char* elsechars = "oongson";
	for(int i = 0;i<7;i++){
		if(name_start[i+1] != elsechars[i]) return false;
	}
	return true;
}

static void parse_seriesID(const char* name_start, size_t length, int* seriesID){
	for (size_t i = 0; i < CPUINFO_COUNT_OF(loongson_name_map_seriesID); i++) {
		const struct cpuinfo_loongarch_seriesID *cur = &loongson_name_map_seriesID[i];
		if (strncmp(cur->prefix, name_start, strlen(cur->prefix)))
			continue;
		cpuinfo_log_debug(
			"found /proc/cpuinfo model name second string \"%.*s\" in loongson seriesID table",
			(int) length, name_start);
		/* Create chipset name from entry */
		*seriesID = cur->seriesID;
		break;
	}
}

static void parse_model_name(
	const char* model_name_start,
	const char* model_name_end,
	struct cpuinfo_loongarch_linux_processor processor[restrict static 1])
{	
	const char* separator = model_name_start;
	for (; separator != model_name_end; separator++) {
		if (*separator == '-') {
			break;
		}
	}
	
	const size_t model_length = (size_t) (separator - model_name_start);
	const size_t name_length = (size_t) (model_name_end - (separator+1));

	size_t value_length = name_length;

	if (value_length > CPUINFO_HARDWARE_VALUE_MAX) {
		cpuinfo_log_info(
			"length of model name value \"%.*s\" in /proc/cpuinfo exceeds limit (%d): truncating to the limit",
			(int) value_length, separator+1, CPUINFO_HARDWARE_VALUE_MAX);
		value_length = CPUINFO_HARDWARE_VALUE_MAX;
	}

	cpuinfo_log_debug("parsed /proc/cpuinfo model name second value = \"%.*s\"", (int) value_length, separator+1);

	if (model_length != 8) {
		cpuinfo_log_warning("Model %.*s in /proc/cpuinfo is ignored due to unexpected length (%zu)",
			(int) model_length, model_name_start, separator - 1);
		return;
	}
	if (name_length < 6 || name_length > 7) {
		cpuinfo_log_warning("Model %.*s in /proc/cpuinfo is ignored due to unexpected length (%zu)",
			(int) name_length, separator + 1, model_name_end);
		return;
	}
	
	/* Verify the presence of hex prefix */
	bool is_loongson = parse_loongson(model_name_start, model_length);
	if (is_loongson) {
		processor->prid = prid_set_companyID(processor->prid, prid_company_loongson);
		processor->flags |= CPUINFO_LOONGARCH_LINUX_VALID_COMPANYID;
	} else {
		cpuinfo_log_warning("Model %.*s in /proc/cpuinfo is ignored due to unexpected words",
			(int) model_length, model_name_start);
		return;
	}

	uint32_t prid_seriesID = 0;

	parse_seriesID(separator + 1, name_length, &prid_seriesID);
	processor->prid = prid_set_seriesID(processor->prid, prid_seriesID);
	processor->flags |= CPUINFO_LOONGARCH_LINUX_VALID_SERIESID;

}

static void parse_cpu_revision(
	const char* cpu_revision_start,
	const char* cpu_revision_end,
	struct cpuinfo_loongarch_linux_processor processor[restrict static 1])
{
	const size_t cpu_revision_length = cpu_revision_end - cpu_revision_start;

	if (cpu_revision_length != 4) {
		cpuinfo_log_warning("CPU revision %.*s in /proc/cpuinfo is ignored due to unexpected length (%zu)",
			(int) cpu_revision_length, cpu_revision_start, cpu_revision_length);
		return;
	}

	/* Skip if there is no hex prefix (0x) */
	if (cpu_revision_start[0] != '0' || cpu_revision_start[1] != 'x') {
		cpuinfo_log_warning("CPU revision %.*s in /proc/cpuinfo is ignored due to lack of 0x prefix",
			(int) cpu_revision_length, cpu_revision_start);
		return;
	}

	/* Check if the value after hex prefix is indeed a hex digit and decode it. */
	char digit_char = cpu_revision_start[2];
	uint32_t cpu_revision = 0;
	if ((uint32_t) (digit_char - '0') < 10) {
		cpu_revision = (uint32_t) (digit_char - '0');
	} else if ((uint32_t) (digit_char - 'A') < 6) {
		cpu_revision = 10 + (uint32_t) (digit_char - 'A');
	} else if ((uint32_t) (digit_char - 'a') < 6) {
		cpu_revision = 10 + (uint32_t) (digit_char - 'a');
	} else {
		cpuinfo_log_warning("CPU revision %.*s in /proc/cpuinfo is ignored due to unexpected non-hex character '%c'",
			(int) cpu_revision_length, cpu_revision_start, digit_char);
		return;
	}
	cpu_revision = cpu_revision * 16;

	digit_char = cpu_revision_start[3];
	if ((uint32_t) (digit_char - '0') < 10) {
		cpu_revision = (uint32_t) (digit_char - '0');
	} else if ((uint32_t) (digit_char - 'A') < 6) {
		cpu_revision = 10 + (uint32_t) (digit_char - 'A');
	} else if ((uint32_t) (digit_char - 'a') < 6) {
		cpu_revision = 10 + (uint32_t) (digit_char - 'a');
	} else {
		cpuinfo_log_warning("CPU revision %.*s in /proc/cpuinfo is ignored due to unexpected non-hex character '%c'",
			(int) cpu_revision_length, cpu_revision_start, digit_char);
		return;
	}

	processor->prid = prid_set_productID(processor->prid, cpu_revision);
	processor->flags |= CPUINFO_LOONGARCH_LINUX_VALID_REVISION;
}

static void parse_core(
	const char* cpu_core_start,
	const char* cpu_core_end,
	struct cpuinfo_loongarch_linux_processor processor[restrict static 1])
{
	uint32_t cpu_core = 0;
	for (const char* digit_ptr = cpu_core_start; digit_ptr != cpu_core_end; digit_ptr++) {
		const uint32_t digit = (uint32_t) (*digit_ptr - '0');

		/* Verify that the character in core is a decimal digit */
		if (digit >= 10) {
			cpuinfo_log_warning("core %.*s in /proc/cpuinfo is ignored due to unexpected non-digit character '%c' at offset %zu",
				(int) (cpu_core_end - cpu_core_start), cpu_core_start,
				*digit_ptr, (size_t) (digit_ptr - cpu_core_start));
			return;
		}

		cpu_core = cpu_core * 10 + digit;
	}

	processor->core_id = cpu_core;
	processor->flags |= CPUINFO_LINUX_FLAG_CORE_ID;
}

static void parse_package(
	const char* cpu_package_start,
	const char* cpu_package_end,
	struct cpuinfo_loongarch_linux_processor processor[restrict static 1])
{
	uint32_t cpu_package = 0;
	for (const char* digit_ptr = cpu_package_start; digit_ptr != cpu_package_end; digit_ptr++) {
		const uint32_t digit = (uint32_t) (*digit_ptr - '0');

		/* Verify that the character in package is a decimal digit */
		if (digit >= 10) {
			cpuinfo_log_warning("package %.*s in /proc/cpuinfo is ignored due to unexpected non-digit character '%c' at offset %zu",
				(int) (cpu_package_end - cpu_package_start), cpu_package_start,
				*digit_ptr, (size_t) (digit_ptr - cpu_package_start));
			return;
		}

		cpu_package = cpu_package * 10 + digit;
	}

	processor->package_id = cpu_package;
	processor->flags |= CPUINFO_LINUX_FLAG_PACKAGE_ID;
}

struct proc_cpuinfo_parser_state {
	uint32_t processor_index;
	uint32_t max_processors_count;
	struct cpuinfo_loongarch_linux_processor* processors;
	struct cpuinfo_loongarch_linux_processor dummy_processor;
};

/*
 *	Decode a single line of /proc/cpuinfo information.
 *	Lines have format <words-with-spaces>[ ]*:[ ]<space-separated words>
 *	An example of /proc/cpuinfo (from Loongson-3A5000 Loongnix20.6):
 *
 *		system type		: generic-loongson-machine
 *		processor		: 0
 *		package			: 0
 *		core			: 0
 *		cpu family		: Loongson-64bit
 *		model name		: Loongson-3A5000-HV
 *		CPU Revision		: 0x11
 *		FPU Revision		: 0x00
 *		CPU MHz			: 2500.00
 *		BogoMIPS		: 5000.00
 *		TLB entries		: 2112
 *		Address sizes		: 48 bits physical, 48 bits virtual
 *		isa			: loongarch32 loongarch64
 *		features		: cpucfg lam ual fpu lsx lasx crc32 lvz lbt_x86 lbt_arm lbt_mips
 *		hardware watchpoint	: yes, iwatch count: 8, dwatch count: 8
 *
 *	An example of /proc/cpuinfo (from Loongson-3A6000 AOSC OS):
 *		system type		: generic-loongson-machine
 *		processor		: 0
 *		package			: 0
 *		core			: 1
 *		global_id		: 0
 *		CPU Family		: Loongson-64bit
 *		Model Name		: Loongson-3A6000
 *		CPU Revision		: 0x00
 *		FPU Revision		: 0x00
 *		CPU MHz			: 2500.00
 *		BogoMIPS		: 5000.00
 *		TLB Entries		: 2112
 *		Address Sizes		: 48 bits physical, 48 bits virtual
 *		ISA			: loongarch32r loongarch32s loongarch64
 *		Features		: cpucfg lam ual fpu lsx lasx crc32 complex crypto lspw lvz lbt_x86 lbt_arm lbt_mips
 *		Hardware Watchpoint	: yes, iwatch count: 8, dwatch count: 4
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
	struct cpuinfo_loongarch_linux_processor* processors = state->processors;
	struct cpuinfo_loongarch_linux_processor* processor  = &state->dummy_processor;
	if (processor_index < max_processors_count) {
		processor = &processors[processor_index];
	}

	const size_t key_length = key_end - line_start;
	switch (key_length) {
		case 3:
			if (strncasecmp(line_start, "isa", key_length) == 0) {
				/* isa Revision is presently useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		case 4:
			if (strncasecmp(line_start, "core", key_length) == 0) {
				parse_core(value_start, value_end, processor);
			} else {
				goto unknown;
			}
			break;
		case 7:
			if (strncasecmp(line_start, "package", key_length) == 0) {
				parse_package(value_start, value_end, processor);
			} else if (strncasecmp(line_start, "CPU MHz", key_length) == 0) {
				/* CPU MHz is presently useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		case 8:
			if (strncasecmp(line_start, "features", key_length) == 0) {
				parse_features(value_start, value_end, processor);
			} else if (strncasecmp(line_start, "BogoMIPS", key_length) == 0) {
				/* BogoMIPS is useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		case 9:
			if (strncasecmp(line_start, "processor", key_length) == 0) {
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
				if (new_processor_index < max_processors_count) {
					/* Record that the processor was mentioned in /proc/cpuinfo */
					processors[new_processor_index].flags |= CPUINFO_LOONGARCH_LINUX_VALID_PROCESSOR;
				} else {
					/* Log and ignore processor */
					cpuinfo_log_warning("processor %"PRIu32" in /proc/cpuinfo is ignored: index exceeds system limit %"PRIu32,
						new_processor_index, max_processors_count - 1);
				}
				state->processor_index = new_processor_index;
				return true;
			} else if (strncasecmp(line_start, "global_id", key_length) == 0) {
				/* global_id is useless, don't parse */
			} else{
				goto unknown;
			}
			break;
		case 10:
			if (strncasecmp(line_start, "cpu family", key_length) == 0) {
				/* cpu family is presently useless, don't parse */
			} else if (strncasecmp(line_start, "model name", key_length) == 0) {
				memcpy(processor->hardware_name, value_start, min(value_end - value_start, CPUINFO_HARDWARE_VALUE_MAX));
				processor->hardware_name[min(value_end - value_start, CPUINFO_HARDWARE_VALUE_MAX)] = '\0';
				if (!(processor->flags & (CPUINFO_LOONGARCH_LINUX_VALID_COMPANYID | CPUINFO_LOONGARCH_LINUX_VALID_SERIESID)))
					parse_model_name(value_start, value_end, processor);
			} else {
				goto unknown;
			}
			break;		
		case 11:
			if (strncasecmp(line_start, "system type", key_length) == 0) {
				/* system type is presently useless, don't parse */
			} else if (strncasecmp(line_start, "TLB entries", key_length) == 0) {
				/* TLB entries is presently useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		case 12:
			if (strncasecmp(line_start, "CPU Revision", key_length) == 0) {
				if (!(processor->flags & CPUINFO_LOONGARCH_LINUX_VALID_REVISION))
					parse_cpu_revision(value_start, value_end, processor);
			} else if (strncasecmp(line_start, "FPU Revision", key_length) == 0) {
				/* FPU Revision is presently useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		case 13:
			if (strncasecmp(line_start, "Address sizes", key_length) == 0) {
				/* Address sizes is presently useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		case 18:
			if (strncasecmp(line_start, "hardware watchpoint", key_length) == 0) {
				/* Address sizes is presently useless, don't parse */
			} else {
				goto unknown;
			}
			break;
		default:
		unknown:
			cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int) key_length, line_start);

	}
	return true;
}

bool cpuinfo_loongarch_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_loongarch_linux_processor processors[restrict static max_processors_count])
{
	struct proc_cpuinfo_parser_state state = {
		.processor_index = 0,
		.max_processors_count = max_processors_count,
		.processors = processors,
	};
	cpuinfo_log_debug("");
	return cpuinfo_linux_parse_multiline_file("/proc/cpuinfo", BUFFER_SIZE,
		(cpuinfo_line_callback) parse_line, &state);
}
