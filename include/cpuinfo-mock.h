#pragma once
#ifndef CPUINFO_MOCK_H
#define CPUINFO_MOCK_H

#include <cpuinfo.h>

#if !defined(CPUINFO_MOCK) || !(CPUINFO_MOCK)
	#error This header is intended only for test use
#endif


#ifdef __cplusplus
extern "C" {
#endif


#if CPUINFO_ARCH_ARM
	void CPUINFO_ABI cpuinfo_set_fpsid(uint32_t fpsid);
	void CPUINFO_ABI cpuinfo_set_wcid(uint32_t wcid);
#endif /* CPUINFO_ARCH_ARM */

#if (CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64) && defined(__linux__)
	void CPUINFO_ABI cpuinfo_set_proc_cpuinfo_path(const char* path);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CPUINFO_MOCK_H */
