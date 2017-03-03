#pragma once

void cpuinfo_x86_mach_init(void);
void cpuinfo_arm_init(void);

typedef void (*cpuinfo_processor_callback)(uint32_t);
