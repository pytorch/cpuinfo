#pragma once

#include <sched.h>

bool cpuinfo_linux_parse_cpuset(const char* filename, cpu_set_t* cpuset);
