#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <cpuinfo/log.h>
#include <linux/api.h>
#include <x86/linux/api.h>

/*
 * Size, in chars, of the on-stack buffer used for parsing lines of
 * /proc/cpuinfo. This is also the limit on the length of a single line.
 */
#define BUFFER_SIZE 2048

static uint32_t parse_processor_number(const char* processor_start, const char* processor_end) {
	const size_t processor_length = (size_t)(processor_end - processor_start);

	if (processor_length == 0) {
		cpuinfo_log_warning("Processor number in /proc/cpuinfo is ignored: string is empty");
		return 0;
	}

	uint32_t processor_number = 0;
	for (const char* digit_ptr = processor_start; digit_ptr != processor_end; digit_ptr++) {
		const uint32_t digit = (uint32_t)(*digit_ptr - '0');
		if (digit > 10) {
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

/*
 * Decode APIC ID reported by Linux kernel for x86/x86-64 architecture.
 * Example of APIC ID reported in /proc/cpuinfo:
 *
 *		apicid		: 2
 */
static void parse_apic_id(
	const char* apic_start,
	const char* apic_end,
	struct cpuinfo_x86_linux_processor processor[restrict static 1]) {
	uint32_t apic_id = 0;
	for (const char* digit_ptr = apic_start; digit_ptr != apic_end; digit_ptr++) {
		const uint32_t digit = *digit_ptr - '0';
		if (digit >= 10) {
			cpuinfo_log_warning(
				"APIC ID %.*s in /proc/cpuinfo is ignored due to unexpected non-digit character '%c' at offset %zu",
				(int)(apic_end - apic_start),
				apic_start,
				*digit_ptr,
				(size_t)(digit_ptr - apic_start));
			return;
		}

		apic_id = apic_id * 10 + digit;
	}

	processor->apic_id = apic_id;
	processor->flags |= CPUINFO_LINUX_FLAG_APIC_ID;
}

struct proc_cpuinfo_parser_state {
	uint32_t processor_index;
	uint32_t max_processors_count;
	struct cpuinfo_x86_linux_processor* processors;
	struct cpuinfo_x86_linux_processor dummy_processor;
};

/*
 *	Decode a single line of /proc/cpuinfo information.
 *	Lines have format <words-with-spaces>[ ]*:[ ]<space-separated words>
 */
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
		if (*value_start != ' ') {
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

	const uint32_t processor_index = state->processor_index;
	const uint32_t max_processors_count = state->max_processors_count;
	struct cpuinfo_x86_linux_processor* processors = state->processors;
	struct cpuinfo_x86_linux_processor* processor = &state->dummy_processor;
	if (processor_index < max_processors_count) {
		processor = &processors[processor_index];
	}

	const size_t key_length = key_end - line_start;
	switch (key_length) {
		case 6:
			if (memcmp(line_start, "apicid", key_length) == 0) {
				parse_apic_id(value_start, value_end, processor);
			} else {
				goto unknown;
			}
			break;
		case 9:
			if (memcmp(line_start, "processor", key_length) == 0) {
				const uint32_t new_processor_index = parse_processor_number(value_start, value_end);
				if (new_processor_index < processor_index) {
					/* Strange: decreasing processor number
					 */
					cpuinfo_log_warning(
						"unexpectedly low processor number %" PRIu32
						" following processor %" PRIu32 " in /proc/cpuinfo",
						new_processor_index,
						processor_index);
				} else if (new_processor_index > processor_index + 1) {
					/* Strange, but common: skipped
					 * processor $(processor_index + 1) */
					cpuinfo_log_warning(
						"unexpectedly high processor number %" PRIu32
						" following processor %" PRIu32 " in /proc/cpuinfo",
						new_processor_index,
						processor_index);
				}
				if (new_processor_index >= max_processors_count) {
					/* Log and ignore processor */
					cpuinfo_log_warning(
						"processor %" PRIu32
						" in /proc/cpuinfo is ignored: index exceeds system limit %" PRIu32,
						new_processor_index,
						max_processors_count - 1);
				} else {
					processors[new_processor_index].flags |= CPUINFO_LINUX_FLAG_PROC_CPUINFO;
				}
				state->processor_index = new_processor_index;
				return true;
			} else {
				goto unknown;
			}
			break;
		default:
		unknown:
			cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int)key_length, line_start);
	}
	return true;
}

bool cpuinfo_x86_linux_parse_proc_cpuinfo(
	uint32_t max_processors_count,
	struct cpuinfo_x86_linux_processor processors[restrict static max_processors_count]) {
	struct proc_cpuinfo_parser_state state = {
		.processor_index = 0,
		.max_processors_count = max_processors_count,
		.processors = processors,
	};
	return cpuinfo_linux_parse_multiline_file("/proc/cpuinfo", BUFFER_SIZE, parse_line, &state);
}

#ifdef __ANDROID__
bool cpuinfo_x86_linux_parse_cpuid_info(
    struct cpuinfo_x86_cpuid_info* x86_cpuid_info) {
    cpuinfo_log_debug("reading cpuid.info file");
    FILE* fd_info = fopen(CPUID_INFO_FILE, "rb");
    if (!fd_info) {
        cpuinfo_log_warning("failed to open file %s: %s", CPUID_INFO_FILE, strerror(errno));
        return false;
    }

    int ret = fread(x86_cpuid_info, sizeof(struct cpuinfo_x86_cpuid_info), 1, fd_info);
    if (!ret) {
        cpuinfo_log_warning("failed to read cpuid info from %s: %s", CPUID_INFO_FILE, strerror(errno));
    }

    if (fclose(fd_info)) {
        cpuinfo_log_warning("failed to close file %s: %s", CPUID_INFO_FILE, strerror(errno));
    }

    return ret ? true : false;
}

#ifdef DEBUG_CPUINFO
void debug_print_cpuid_info_file (void) {
    cpuinfo_log_debug("cpuinfo: ----------------------------");
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "erms", cpuinfo_has_x86_erms());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "smap", cpuinfo_has_x86_smap());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "serialize", cpuinfo_has_x86_serialize());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "adx", cpuinfo_has_x86_adx());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "aes", cpuinfo_has_x86_aes());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx", cpuinfo_has_x86_avx());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx2", cpuinfo_has_x86_avx2());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512bf16", cpuinfo_has_x86_avx512bf16());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512bitalg", cpuinfo_has_x86_avx512bitalg());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512bw", cpuinfo_has_x86_avx512bw());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512cd", cpuinfo_has_x86_avx512cd());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512dq", cpuinfo_has_x86_avx512dq());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512er", cpuinfo_has_x86_avx512er());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512f", cpuinfo_has_x86_avx512f());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512ifma", cpuinfo_has_x86_avx512ifma());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512pf", cpuinfo_has_x86_avx512pf());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512vbmi", cpuinfo_has_x86_avx512vbmi());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512vbmi2", cpuinfo_has_x86_avx512vbmi2());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512vl", cpuinfo_has_x86_avx512vl());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512vnni", cpuinfo_has_x86_avx512vnni());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512vp2intersect", cpuinfo_has_x86_avx512vp2intersect());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512vpopcntdq", cpuinfo_has_x86_avx512vpopcntdq());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512_4fmaps", cpuinfo_has_x86_avx512_4fmaps());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "avx512_4vnniw", cpuinfo_has_x86_avx512_4vnniw());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "bmi", cpuinfo_has_x86_bmi());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "bmi2", cpuinfo_has_x86_bmi2());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "clwb", cpuinfo_has_x86_clwb());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "clzero", cpuinfo_has_x86_clzero());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "cmpxchg16b", cpuinfo_has_x86_cmpxchg16b());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "f16c", cpuinfo_has_x86_f16c());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "fma3", cpuinfo_has_x86_fma3());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "fma4", cpuinfo_has_x86_fma4());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "gfni", cpuinfo_has_x86_gfni());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "hle", cpuinfo_has_x86_hle());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "lzcnt", cpuinfo_has_x86_lzcnt());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "misaligned_sse", cpuinfo_has_x86_misaligned_sse());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "movbe", cpuinfo_has_x86_movbe());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "mpx", cpuinfo_has_x86_mpx());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "mwait", cpuinfo_has_x86_mwait());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "mwaitx", cpuinfo_has_x86_mwaitx());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "pclmulqdq", cpuinfo_has_x86_pclmulqdq());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "popcnt", cpuinfo_has_x86_popcnt());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "prefetch", cpuinfo_has_x86_prefetch());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "prefetchw", cpuinfo_has_x86_prefetchw());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "prefetchwt1", cpuinfo_has_x86_prefetchwt1());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "rdpid", cpuinfo_has_x86_rdpid());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "rdrand", cpuinfo_has_x86_rdrand());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "rdseed", cpuinfo_has_x86_rdseed());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "rdtscp", cpuinfo_has_x86_rdtscp());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "rtm", cpuinfo_has_x86_rtm());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sha", cpuinfo_has_x86_sha());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sse3", cpuinfo_has_x86_sse3());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sse4a", cpuinfo_has_x86_sse4a());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sse4_1", cpuinfo_has_x86_sse4_1());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sse4_2", cpuinfo_has_x86_sse4_2());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "ssse3", cpuinfo_has_x86_ssse3());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "tbm", cpuinfo_has_x86_tbm());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "three_d_now", cpuinfo_has_x86_3dnow());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "three_d_now_plus", cpuinfo_has_x86_3dnow_plus());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "vaes", cpuinfo_has_x86_vaes());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "vpclmulqdq", cpuinfo_has_x86_vpclmulqdq());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "xop", cpuinfo_has_x86_xop());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "xsave", cpuinfo_has_x86_xsave());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "xtest", cpuinfo_has_x86_xtest());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "cmov", cpuinfo_has_x86_cmov());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "cmpxchg8b", cpuinfo_has_x86_cmpxchg8b());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "daz", cpuinfo_has_x86_daz());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "fpu", cpuinfo_has_x86_fpu());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "mmx", cpuinfo_has_x86_mmx());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "mmx_plus", cpuinfo_has_x86_mmx_plus());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "rdtsc", cpuinfo_has_x86_rdtsc());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sse", cpuinfo_has_x86_sse());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "sse2", cpuinfo_has_x86_sse2());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "three_d_now_geode", cpuinfo_has_x86_3dnow_geode());
    cpuinfo_log_debug("cpuinfo: %-20s: %d", "lahf_sahf", cpuinfo_has_x86_lahf_sahf());
    cpuinfo_log_debug("cpuinfo: ----------------------------");
}
#endif //DEBUG_CPUINFO
#endif //__ANDROID__
