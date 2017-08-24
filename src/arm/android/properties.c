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

static void parse_dalvik_arm_variant(
	const char* value_start,
	const char* value_end,
	enum cpuinfo_dalvik_arm_variant dalvik_arm_variant[restrict static 1])
{
	const size_t value_length = value_end - value_start;
	switch (value_length) {
		case 4:
			if (memcmp(value_start, "kryo", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_kryo;
			}
			break;
		case 5:
			if (memcmp(value_start, "krait", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_krait;
			}
			break;
		case 6:
			if (memcmp(value_start, "denver", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_denver;
			}
			break;
		case 8:
			if (memcmp(value_start, "scorpion", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a5;
			}
			break;
		case 9:
			if (memcmp(value_start, "cortex-a", value_length - 1) == 0) {
				switch (value_start[value_length - 1]) {
					case '5':
						*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a5;
						break;
					case '7':
						*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a7;
						break;
					case '8':
						*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a8;
						break;
					case '9':
						*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a9;
						break;
				}
			}
			break;
		case 10:
			if (memcmp(value_start, "cortex-a15", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a15;
			} else if (memcmp(value_start, "cortex-a53", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a53;
			} else if (memcmp(value_start, "cortex-a72", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a72;
			}
			break;
		case 13:
			if (memcmp(value_start, "cortex-a53,a57", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a53_a57;
			}
			break;
		case 37:
			if (memcmp(value_start, "cortex-a53,cortex-a57,generic,default", value_length) == 0) {
				*dalvik_arm_variant = cpuinfo_dalvik_arm_variant_cortex_a53_a57;
			}
			break;
	}
	cpuinfo_log_debug("parsed dalvik_arm_variant = %d", *dalvik_arm_variant);
}

static void parse_dalvik_arm64_variant(
	const char* value_start,
	const char* value_end,
	enum cpuinfo_dalvik_arm64_variant dalvik_arm64_variant[restrict static 1])
{
	const size_t value_length = value_end - value_start;
	switch (value_length) {
		case 4:
			if (memcmp(value_start, "kryo", value_length) == 0) {
				*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_kryo;
			}
			break;
		case 8:
			if (memcmp(value_start, "denver64", value_length) == 0) {
				*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_denver64;
			}
			break;
		case 9:
			if (memcmp(value_start, "exynos-m", value_length - 1) == 0) {
				switch (value_start[value_length - 1]) {
					case '1':
						*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_exynos_m1;
						break;
					case '2':
						*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_exynos_m2;
						break;
				}	
			}
		case 10:
			if (memcmp(value_start, "cortex-a53", value_length) == 0) {
				*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_cortex_a53;
			} else if (memcmp(value_start, "cortex-a57", value_length) == 0) {
				*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_cortex_a57;
			}
			break;
		case 14:
			if (memcmp(value_start, "cortex-a53.a57", value_length) == 0) {
				*dalvik_arm64_variant = cpuinfo_dalvik_arm64_variant_cortex_a53_a57;
			}
			break;
	}
	cpuinfo_log_debug("parsed dalvik_arm64_variant = %d", *dalvik_arm64_variant);
}

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
			if (memcmp(line_start, "mediatek.platform", key_length) == 0) {
				memcpy(properties->ro_mediatek_platform, value_start, limited_value_length);
				cpuinfo_log_debug("parsed ro.mediatek.platform = \"%s\"",
					properties->ro_mediatek_platform);
			} else if (memcmp(line_start, "ro.board.platform", key_length) == 0) {
				memcpy(properties->ro_board_platform, value_start, limited_value_length);
				cpuinfo_log_debug("parsed ro.board.platform = \"%s\"",
					properties->ro_board_platform);
			}
			break;
		case 25:
			if (memcmp(line_start, "dalvik.vm.isa.arm.variant", key_length) == 0) {
				parse_dalvik_arm_variant(value_start, value_end, &properties->dalvik_arm_variant);
			}
			break;
		case 27:
			if (memcmp(line_start, "dalvik.vm.isa.arm64.variant", key_length) == 0) {
				parse_dalvik_arm64_variant(value_start, value_end, &properties->dalvik_arm64_variant);
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
