#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <linux/api.h>
#include <arm/android/api.h>
#include <arm/linux/api.h>
#include <log.h>

/*
 * Size, in chars, of the on-stack buffer used for parsing lines of /system/build.prop
 * This is also the limit on the length of a single line.
 */
#define BUFFER_SIZE 1024

/*
 *	Decode a single line of /system/build.prop information.
 *	Lines have format <key>=<value>
 *	An truncated example of /system/build.prop (from Galaxy S7):
 *
 *      # ro.product.cpu.abi and ro.product.cpu.abi2 are obsolete,
 *      # use ro.product.cpu.abilist instead.
 *      ro.product.cpu.abi=arm64-v8a
 *      ro.product.cpu.abilist=arm64-v8a,armeabi-v7a,armeabi
 *      ro.product.cpu.abilist32=armeabi-v7a,armeabi
 *      ro.product.cpu.abilist64=arm64-v8a
 *      ro.product.manufacturer=samsung
 */
static bool parse_line(
	const char* line_start,
	const char* line_end,
	struct cpuinfo_android_properties properties[restrict static 1],
	uint64_t line_number)
{
	/* Minimal valid line length is 3 characters: 1-character key, '=', 1-character value. */
	const size_t line_length = line_end - line_start;
	if (line_length < 3) {
		return true;
	}

	/* Lines starting with '#' are comments */
	if (line_start[0] == '#') {
		return true;
	}
	
	/* Search for '=' on the line. */
	const char* separator = line_start;
	for (; separator != line_end; separator++) {
		if (*separator == '=') {
			break;
		}
	}
	/* Skip line if no '=' separator was found. */
	if (separator == line_end) {
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
		return true;
	}

	/* Skip trailing spaces in value part (if any) */
	const char* value_end = line_end;
	for (; value_end != value_start; value_end--) {
		if (value_end[-1] != ' ') {
			break;
		}
	}

	const size_t key_length = separator - line_start;
	const size_t value_length = value_end - value_start;
	const size_t limited_value_length = (value_length >= CPUINFO_BUILD_PROP_VALUE_MAX) ?
		CPUINFO_BUILD_PROP_VALUE_MAX - 1 : value_length;
	switch (key_length) {
		case 11:
			if (memcmp(line_start, "ro.chipname", key_length) == 0) {
				memcpy(properties->ro_chipname, value_start, limited_value_length);
				cpuinfo_log_debug("parsed ro.chipname = \"%s\"",
					properties->ro_chipname);
			}
			break;
		case 16:
			if (memcmp(line_start, "ro.product.board", key_length) == 0) {
				memcpy(properties->ro_product_board, value_start, limited_value_length);
				cpuinfo_log_debug("parsed ro.product.board = \"%s\"",
					properties->ro_product_board);
			}
			break;
		case 17:
			if (memcmp(line_start, "ro.board.platform", key_length) == 0) {
				memcpy(properties->ro_board_platform, value_start, limited_value_length);
				cpuinfo_log_debug("parsed ro.board.platform = \"%s\"",
					properties->ro_board_platform);
			}
			break;
		case 20:
			if (memcmp(line_start, "ro.mediatek.platform", key_length) == 0) {
				memcpy(properties->ro_mediatek_platform, value_start, limited_value_length);
				cpuinfo_log_debug("parsed ro.mediatek.platform = \"%s\"",
					properties->ro_mediatek_platform);
			}
			break;
	}
	return true;
}

bool cpuinfo_arm_android_parse_properties(struct cpuinfo_android_properties properties[restrict static 1]) {
	memset(properties, 0, sizeof(struct cpuinfo_android_properties));
	return cpuinfo_linux_parse_multiline_file("/system/build.prop", BUFFER_SIZE,
		(cpuinfo_line_callback) parse_line, properties);
}
