#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <cpuinfo.h>

extern struct cpuinfo_cache* cpuinfo_cache[cpuinfo_cache_level_max];
extern uint32_t cpuinfo_cache_count[cpuinfo_cache_level_max];
extern uint32_t cpuinfo_processors_count;

void cpuinfo_x86_mach_init(void);
void cpuinfo_x86_linux_init(void);
void cpuinfo_arm_linux_init(void);

typedef void (*cpuinfo_processor_callback)(uint32_t);
