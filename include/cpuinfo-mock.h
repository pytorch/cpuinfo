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


#if CPUINFO_ARCH_ARM && defined(__linux__)
	void CPUINFO_ABI cpuinfo_set_proc_cpuinfo_path(const char* path);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CPUINFO_MOCK_H */
