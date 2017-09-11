#pragma once

#include <cpuinfo.h>
#include <arm/api.h>
#include <arm/linux/api.h>

/* As per include/sys/system_properties.h in Android NDK */
#define CPUINFO_BUILD_PROP_NAME_MAX  32
#define CPUINFO_BUILD_PROP_VALUE_MAX 92

struct cpuinfo_android_properties {
	char proc_cpuinfo_hardware[CPUINFO_HARDWARE_VALUE_MAX];
	char ro_product_board[CPUINFO_BUILD_PROP_VALUE_MAX];
	char ro_board_platform[CPUINFO_BUILD_PROP_VALUE_MAX];
	char ro_mediatek_platform[CPUINFO_BUILD_PROP_VALUE_MAX];
	char ro_chipname[CPUINFO_BUILD_PROP_VALUE_MAX];
};

enum cpuinfo_android_chipset_property {
	cpuinfo_android_chipset_property_proc_cpuinfo_hardware = 0,
	cpuinfo_android_chipset_property_ro_product_board,
	cpuinfo_android_chipset_property_ro_board_platform,
	cpuinfo_android_chipset_property_ro_mediatek_platform,
	cpuinfo_android_chipset_property_ro_chipname,
	cpuinfo_android_chipset_property_max,
};

bool cpuinfo_arm_android_parse_properties(struct cpuinfo_android_properties properties[restrict static 1]);


struct cpuinfo_arm_chipset cpuinfo_arm_android_decode_chipset(
	const struct cpuinfo_android_properties properties[restrict static 1],
	uint32_t cores,
	uint32_t max_cpu_freq_max);

struct cpuinfo_arm_chipset cpuinfo_arm_android_decode_chipset_from_proc_cpuinfo_hardware(
	const char proc_cpuinfo_hardware[restrict static CPUINFO_HARDWARE_VALUE_MAX],
	uint32_t cores, uint32_t max_cpu_freq_max, bool is_tegra);
struct cpuinfo_arm_chipset cpuinfo_arm_android_decode_chipset_from_ro_product_board(
	const char ro_product_board[restrict static CPUINFO_BUILD_PROP_VALUE_MAX],
	uint32_t cores, uint32_t max_cpu_freq_max);
struct cpuinfo_arm_chipset cpuinfo_arm_android_decode_chipset_from_ro_board_platform(
	const char ro_board_platform[restrict static CPUINFO_BUILD_PROP_VALUE_MAX],
	uint32_t cores, uint32_t max_cpu_freq_max);
struct cpuinfo_arm_chipset cpuinfo_arm_android_decode_chipset_from_ro_mediatek_platform(
	const char ro_mediatek_platform[restrict static CPUINFO_BUILD_PROP_VALUE_MAX]);
struct cpuinfo_arm_chipset cpuinfo_arm_android_decode_chipset_from_ro_chipname(
	const char ro_chipname[restrict static CPUINFO_BUILD_PROP_VALUE_MAX]);
