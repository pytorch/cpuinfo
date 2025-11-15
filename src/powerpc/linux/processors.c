#include <cpuinfo/log.h>
#include <linux/api.h>
#include <powerpc/linux/api.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__ANDROID__)
/*
 *  * sched.h is only used for CPU_SETSIZE constant.
 *   * Android NDK headers before platform 21 do have this constant in sched.h
 *    */
#include <sched.h>
#endif

#define STRINGIFY(token) #token
#define KERNEL_MAX_FILENAME "/sys/devices/system/cpu/kernel_max"
#define PROCESSOR_ONLINE_FILENAME_SIZE (sizeof("/sys/devices/system/cpu/cpu" STRINGIFY(UINT32_MAX) "/online"))
#define PROCESSOR_ONLINE_FILENAME_FORMAT "/sys/devices/system/cpu/cpu%" PRIu32 "/online"
#define PROCESSOR_ONLINE_FILESIZE 32

/* Locale-independent */
inline static bool is_whitespace(char c) {
	switch (c) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;
		default:
			return false;
	}
}

inline static const char* parse_number(const char* string, const char* end, uint32_t number_ptr[restrict static 1]) {
	uint32_t number = 0;
	while (string != end) {
		const uint32_t digit = (uint32_t)(*string) - (uint32_t)'0';
		if (digit >= 10) {
			break;
		}
		number = number * UINT32_C(10) + digit;
		string += 1;
	}
	*number_ptr = number;
	return string;
}

static bool uint32_parser(const char* filename, const char* text_start, const char* text_end, void* context) {
	if (text_start == text_end) {
		cpuinfo_log_error("failed to parse file %s: file is empty", KERNEL_MAX_FILENAME);
		return false;
	}

	uint32_t kernel_max = 0;
	const char* parsed_end = parse_number(text_start, text_end, &kernel_max);
	if (parsed_end == text_start) {
		cpuinfo_log_error(
			"failed to parse file %s: \"%.*s\" is not an unsigned number",
			filename,
			(int)(text_end - text_start),
			text_start);
		return false;
	} else {
		for (const char* char_ptr = parsed_end; char_ptr != text_end; char_ptr++) {
			if (!is_whitespace(*char_ptr)) {
				cpuinfo_log_warning(
					"non-whitespace characters \"%.*s\" following number in file %s are ignored",
					(int)(text_end - char_ptr),
					char_ptr,
					filename);
				break;
			}
		}
	}

	uint32_t* kernel_max_ptr = (uint32_t*)context;
	*kernel_max_ptr = kernel_max;
	return true;
}

bool cpuinfo_linux_get_processor_online_status(uint32_t processor, uint32_t* online_status_ptr) {
	char processor_online_filename[PROCESSOR_ONLINE_FILENAME_SIZE];
	const int chars_formatted = snprintf(
		processor_online_filename, PROCESSOR_ONLINE_FILENAME_SIZE, PROCESSOR_ONLINE_FILENAME_FORMAT, processor);
	if ((unsigned int)chars_formatted >= PROCESSOR_ONLINE_FILENAME_SIZE) {
		cpuinfo_log_warning("failed to format filename for online status of processor %" PRIu32, processor);
		return 0;
	}
	uint32_t online_status;
	if (cpuinfo_linux_parse_small_file(
		    processor_online_filename, PROCESSOR_ONLINE_FILESIZE, uint32_parser, &online_status)) {
		cpuinfo_log_debug(
			"parsed online status value of %" PRIu32 " for logical processor %" PRIu32 " from %s",
			online_status,
			processor,
			processor_online_filename);
		*online_status_ptr = online_status;
		return true;
	} else {
		cpuinfo_log_info(
			"failed to parse online status for processor %" PRIu32 " from %s",
			processor,
			processor_online_filename);
		return false;
	}
}
