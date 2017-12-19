#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <cpuinfo.h>


#define CPUINFO_LINUX_FLAG_PRESENT            UINT32_C(0x00000001)
#define CPUINFO_LINUX_FLAG_POSSIBLE           UINT32_C(0x00000002)
#define CPUINFO_LINUX_MASK_USABLE             UINT32_C(0x00000003)
#define CPUINFO_LINUX_FLAG_MAX_FREQUENCY      UINT32_C(0x00000004)
#define CPUINFO_LINUX_FLAG_MIN_FREQUENCY      UINT32_C(0x00000008)
#define CPUINFO_LINUX_FLAG_SMT_ID             UINT32_C(0x00000010)
#define CPUINFO_LINUX_FLAG_CORE_ID            UINT32_C(0x00000020)
#define CPUINFO_LINUX_FLAG_PACKAGE_ID         UINT32_C(0x00000040)
#define CPUINFO_LINUX_FLAG_APIC_ID            UINT32_C(0x00000080)
#define CPUINFO_LINUX_FLAG_SMT_CLUSTER        UINT32_C(0x00000100)
#define CPUINFO_LINUX_FLAG_CORE_CLUSTER       UINT32_C(0x00000200)
#define CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER    UINT32_C(0x00000400)


typedef bool (*cpuinfo_cpulist_callback)(uint32_t, uint32_t, void*);
bool cpuinfo_linux_parse_cpulist(const char* filename, cpuinfo_cpulist_callback callback, void* context);
typedef bool (*cpuinfo_smallfile_callback)(const char*, const char*, void*);
bool cpuinfo_linux_parse_small_file(const char* filename, size_t buffer_size, cpuinfo_smallfile_callback, void* context);
typedef bool (*cpuinfo_line_callback)(const char*, const char*, void*, uint64_t);
bool cpuinfo_linux_parse_multiline_file(const char* filename, size_t buffer_size, cpuinfo_line_callback, void* context);

uint32_t cpuinfo_linux_get_max_processors_count(void);
uint32_t cpuinfo_linux_get_max_possible_processor(uint32_t max_processors_count);
uint32_t cpuinfo_linux_get_max_present_processor(uint32_t max_processors_count);
uint32_t cpuinfo_linux_get_processor_min_frequency(uint32_t processor);
uint32_t cpuinfo_linux_get_processor_max_frequency(uint32_t processor);
bool cpuinfo_linux_get_processor_package_id(uint32_t processor, uint32_t package_id[restrict static 1]);
bool cpuinfo_linux_get_processor_core_id(uint32_t processor, uint32_t core_id[restrict static 1]);

bool cpuinfo_linux_detect_possible_processors(uint32_t max_processors_count,
	uint32_t* processor0_flags, uint32_t processor_struct_size, uint32_t possible_flag);
bool cpuinfo_linux_detect_present_processors(uint32_t max_processors_count,
	uint32_t* processor0_flags, uint32_t processor_struct_size, uint32_t present_flag);

typedef bool (*cpuinfo_siblings_callback)(uint32_t, uint32_t, uint32_t, void*);
bool cpuinfo_linux_detect_core_siblings(
	uint32_t max_processors_count,
	uint32_t processor,
	cpuinfo_siblings_callback callback,
	void* context);
bool cpuinfo_linux_detect_thread_siblings(
	uint32_t max_processors_count,
	uint32_t processor,
	cpuinfo_siblings_callback callback,
	void* context);

enum cpuinfo_android_gpu_vendor {
	cpuinfo_android_gpu_vendor_unknown = 0,
	cpuinfo_android_gpu_vendor_arm,
	cpuinfo_android_gpu_vendor_broadcom,
	cpuinfo_android_gpu_vendor_intel,
	cpuinfo_android_gpu_vendor_nvidia,
	cpuinfo_android_gpu_vendor_powervr,
	cpuinfo_android_gpu_vendor_qualcomm,
	cpuinfo_android_gpu_vendor_vivante,
	cpuinfo_android_gpu_vendor_max,
};

enum cpuinfo_android_gpu_series {
	cpuinfo_android_gpu_series_unknown = 0,
	cpuinfo_android_gpu_series_arm_mali,
	cpuinfo_android_gpu_series_arm_mali_t,
	cpuinfo_android_gpu_series_arm_mali_g,
	cpuinfo_android_gpu_series_broadcom_videocore,
	cpuinfo_android_gpu_series_intel_gen,
	cpuinfo_android_gpu_series_intel_hd_graphics,
	cpuinfo_android_gpu_series_nvidia_tegra,
	cpuinfo_android_gpu_series_nvidia_tegra_k,
	cpuinfo_android_gpu_series_nvidia_tegra_x,
	cpuinfo_android_gpu_series_powervr_sgx,
	cpuinfo_android_gpu_series_powervr_rogue_g,
	cpuinfo_android_gpu_series_powervr_rogue_ge,
	cpuinfo_android_gpu_series_powervr_rogue_gm,
	cpuinfo_android_gpu_series_powervr_rogue_gt,
	cpuinfo_android_gpu_series_powervr_rogue_gx,
	cpuinfo_android_gpu_series_qualcomm_adreno,
	cpuinfo_android_gpu_series_vivante_gc,
	cpuinfo_android_gpu_series_max,
};

#define CPUINFO_ANDROID_GPU_SUFFIX_MAX 8

struct cpuinfo_android_gpu {
	enum cpuinfo_android_gpu_vendor vendor;
	enum cpuinfo_android_gpu_series series;
	uint32_t model;
	char suffix[CPUINFO_ANDROID_GPU_SUFFIX_MAX];
};

struct cpuinfo_android_gpu cpuinfo_android_decode_gpu(const char* renderer);
void cpuinfo_android_gpu_to_string(
	const struct cpuinfo_android_gpu gpu[restrict static 1],
	char name[restrict static CPUINFO_GPU_NAME_MAX]);

extern const struct cpuinfo_processor** cpuinfo_linux_cpu_to_processor_map;
extern const struct cpuinfo_core** cpuinfo_linux_cpu_to_core_map;
