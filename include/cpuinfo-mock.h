#pragma once
#ifndef CPUINFO_MOCK_H
#define CPUINFO_MOCK_H

#include <cpuinfo.h>
#if defined(__linux__)
	#include <sys/types.h>
#endif


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

struct cpuinfo_mock_file {
	const char* path;
	size_t size;
	const char* content;
	size_t offset;
};

struct cpuinfo_mock_property {
	const char* key;
	const char* value;
};

#if defined(__linux__)
	void CPUINFO_ABI cpuinfo_mock_filesystem(struct cpuinfo_mock_file* files);
	int CPUINFO_ABI cpuinfo_mock_open(const char* path, int oflag);
	int CPUINFO_ABI cpuinfo_mock_close(int fd);
	ssize_t CPUINFO_ABI cpuinfo_mock_read(int fd, void* buffer, size_t capacity);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CPUINFO_MOCK_H */
