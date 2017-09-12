#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <dlfcn.h>

#include <linux/api.h>
#include <arm/android/api.h>
#include <arm/linux/api.h>
#include <log.h>

#if CPUINFO_MOCK
	#include <cpuinfo-mock.h>
#endif


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

#if CPUINFO_MOCK
	static struct cpuinfo_mock_property* cpuinfo_mock_properties = NULL;

	void CPUINFO_ABI cpuinfo_mock_android_properties(struct cpuinfo_mock_property* properties) {
		cpuinfo_log_info("Android properties mocking enabled");
		cpuinfo_mock_properties = properties;
	}

	static int mock_system_property_get(const char* key, char* value) {
		for (const struct cpuinfo_mock_property* prop = cpuinfo_mock_properties; prop->key != NULL; prop++) {
			if (strncmp(key, prop->key, CPUINFO_BUILD_PROP_NAME_MAX) == 0) {
				strncpy(value, prop->value, CPUINFO_BUILD_PROP_VALUE_MAX);
				return (int) strnlen(prop->value, CPUINFO_BUILD_PROP_VALUE_MAX);
			}
		}
		*value = '\0';
		return 0;
	}

	static bool cpuinfo_arm_android_query_properties(struct cpuinfo_android_properties properties[restrict static 1]) {
		if (cpuinfo_mock_properties == NULL) {
			return false;
		}

		const int ro_product_board_length =
			mock_system_property_get("ro.product.board", properties->ro_product_board);
		cpuinfo_log_debug("read ro.product.board = \"%.*s\"",
			ro_product_board_length, properties->ro_product_board);

		const int ro_board_platform_length =
			mock_system_property_get("ro.board.platform", properties->ro_board_platform);
		cpuinfo_log_debug("read ro.board.platform = \"%.*s\"",
			ro_board_platform_length, properties->ro_board_platform);

		const int ro_mediatek_platform_length =
			mock_system_property_get("ro.mediatek.platform", properties->ro_mediatek_platform);
		cpuinfo_log_debug("read ro.mediatek.platform = \"%.*s\"",
			ro_mediatek_platform_length, properties->ro_mediatek_platform);

		const int ro_chipname_length =
			mock_system_property_get("ro.chipname", properties->ro_chipname);
		cpuinfo_log_debug("read ro.chipname = \"%.*s\"",
			ro_chipname_length, properties->ro_chipname);

		return true;
	}
#else
	/*
	 * Function pointer declaration for
	 *     int __system_property_get(const char *name, char *value)
	 * function from sys/system_properties.h header. Google's stance on whether function is part of a stable API is unclear:
	 * it was removed from public API in some NDK versions, and then added back. In practice, libc.so has this symbol,
	 * so we dynamically locate this function using dlopen + dlsym.
	 */
	typedef int (*system_property_get_ptr)(const char[], char[]);

	static bool cpuinfo_arm_android_query_properties(struct cpuinfo_android_properties properties[restrict static 1]) {
		void* libc = dlopen("libc.so", RTLD_LAZY);
		if (libc == NULL) {
			cpuinfo_log_warning("failed to dlopen libc.so; fallback to reading /system/build.prop for Android properties");
			return false;
		}

		system_property_get_ptr system_property_get = (system_property_get_ptr) dlsym(libc, "__system_property_get");
		if (system_property_get == NULL) {
			cpuinfo_log_warning(
				"could not find __system_property_get in libc.so: "
				"fallback to reading /system/build.prop for Android properties");
			goto cleanup;
		}

		const int ro_product_board_length = system_property_get("ro.product.board", properties->ro_product_board);
		cpuinfo_log_debug("read ro.product.board = \"%.*s\"", ro_product_board_length, properties->ro_product_board);

		const int ro_board_platform_length = system_property_get("ro.board.platform", properties->ro_board_platform);
		cpuinfo_log_debug("read ro.board.platform = \"%.*s\"", ro_board_platform_length, properties->ro_board_platform);

		const int ro_mediatek_platform_length =
			system_property_get("ro.mediatek.platform", properties->ro_mediatek_platform);
		cpuinfo_log_debug("read ro.mediatek.platform = \"%.*s\"",
			ro_mediatek_platform_length, properties->ro_mediatek_platform);

		const int ro_chipname_length = system_property_get("ro.chipname", properties->ro_chipname);
		cpuinfo_log_debug("read ro.chipname = \"%.*s\"", ro_chipname_length, properties->ro_chipname);

	cleanup:
		dlclose(libc);
		return system_property_get != NULL;
	}
#endif

bool cpuinfo_arm_android_parse_properties(struct cpuinfo_android_properties properties[restrict static 1]) {
	memset(properties, 0, sizeof(struct cpuinfo_android_properties));

	/*
	 * First, try to use __system_property_get, which is widely supported. Whether is it a part of public API is
	 * debatable: Android 7.0 release note suggest to use __system_property_get instead of property_get from
	 * libcutils.so, but it was removed from headers in some NDK versions on ARM64.
	 * If __system_property_get can't be located, fallback to parsing /system/build.prop file. This method is
	 * recommended by  an Android engineer (see https://stackoverflow.com/a/28416743), but doesn't work on Android Oreo
	 * (where the /system/build.prop file is not readable for non-root users) and some Huawei devices (where
	 * /system/build.prop contains empty strings for some properties of interest).
	 */
	return cpuinfo_arm_android_query_properties(properties) ||
		cpuinfo_linux_parse_multiline_file("/system/build.prop", BUFFER_SIZE,
			(cpuinfo_line_callback) parse_line, properties);
}
