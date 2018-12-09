#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <cpuinfo.h>

#ifdef _WIN32
	#include <windows.h>
#endif

enum cpuinfo_cache_level {
	cpuinfo_cache_level_1i  = 0,
	cpuinfo_cache_level_1d  = 1,
	cpuinfo_cache_level_2   = 2,
	cpuinfo_cache_level_3   = 3,
	cpuinfo_cache_level_4   = 4,
	cpuinfo_cache_level_max = 5,
};

extern bool cpuinfo_is_initialized;
extern struct cpuinfo_processor* cpuinfo_processors;
extern struct cpuinfo_core* cpuinfo_cores;
extern struct cpuinfo_cluster* cpuinfo_clusters;
extern struct cpuinfo_package* cpuinfo_packages;
extern struct cpuinfo_cache* cpuinfo_cache[cpuinfo_cache_level_max];
extern uint32_t cpuinfo_processors_count;
extern uint32_t cpuinfo_cores_count;
extern uint32_t cpuinfo_clusters_count;
extern uint32_t cpuinfo_packages_count;
extern uint32_t cpuinfo_cache_count[cpuinfo_cache_level_max];

void cpuinfo_x86_mach_init(void);
void cpuinfo_x86_linux_init(void);
#ifdef _WIN32
	BOOL CALLBACK cpuinfo_x86_windows_init(PINIT_ONCE init_once, PVOID parameter, PVOID* context);
#endif
void cpuinfo_arm_mach_init(void);
void cpuinfo_arm_linux_init(void);

typedef void (*cpuinfo_processor_callback)(uint32_t);
