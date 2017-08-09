#pragma once

#include <stdbool.h>


#define CPUINFO_LINUX_FLAG_PRESENT            UINT32_C(0x00000001)
#define CPUINFO_LINUX_FLAG_POSSIBLE           UINT32_C(0x00000002)
#define CPUINFO_LINUX_MASK_USABLE             UINT32_C(0x00000003)
#define CPUINFO_LINUX_FLAG_ONLINE             UINT32_C(0x00000004)
#define CPUINFO_LINUX_FLAG_MAX_FREQUENCY      UINT32_C(0x00000010)
#define CPUINFO_LINUX_FLAG_MIN_FREQUENCY      UINT32_C(0x00000020)
#define CPUINFO_LINUX_FLAG_CORE_ID            UINT32_C(0x00000100)
#define CPUINFO_LINUX_FLAG_CORE_CLUSTER       UINT32_C(0x00000200)
#define CPUINFO_LINUX_FLAG_PACKAGE_ID         UINT32_C(0x00000400)
#define CPUINFO_LINUX_FLAG_PACKAGE_CLUSTER    UINT32_C(0x00000800)


typedef bool (*cpuinfo_cpulist_callback)(uint32_t, uint32_t, void*);
bool cpuinfo_linux_parse_cpulist(const char* filename, cpuinfo_cpulist_callback callback, void* context);
typedef bool (*cpuinfo_smallfile_callback)(const char*, const char*, void*);
bool cpuinfo_linux_parse_small_file(const char* filename, size_t buffer_size, cpuinfo_smallfile_callback, void* context);

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

bool cpuinfo_linux_detect_core_siblings(
	uint32_t max_processors_count,
	uint32_t processor,
	uint32_t* processor0_flags,
	uint32_t* processor0_package_id,
	uint32_t* processor0_package_group_min,
	uint32_t* processor0_package_group_max,
	uint32_t processor_struct_size);
bool cpuinfo_linux_detect_thread_siblings(
	uint32_t max_processors_count,
	uint32_t processor,
	uint32_t* processor0_flags,
	uint32_t* processor0_core_id,
	uint32_t* processor0_core_group_min,
	uint32_t* processor0_core_group_max,
	uint32_t processor_struct_size);
