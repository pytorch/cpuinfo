#pragma once

#include <stdbool.h>

#include <cpuinfo.h>

bool cpuinfo_gpu_query_gles2(char name[restrict static CPUINFO_GPU_NAME_MAX]);
void cpuinfo_gpu_ios_query_gles2(char name[restrict static CPUINFO_GPU_NAME_MAX]);
