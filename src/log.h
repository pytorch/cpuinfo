#pragma once

#include <inttypes.h>

#define CPUINFO_LOG_ERROR 1
#define CPUINFO_LOG_WARNING 2
#define CPUINFO_LOG_INFO 3
#define CPUINFO_LOG_DEBUG 4

#ifndef CPUINFO_LOG_LEVEL
	#define CPUINFO_LOG_LEVEL CPUINFO_LOG_ERROR
#endif

#ifdef __GNUC__
__attribute__((__format__(__printf__, 1, 2)))
#endif
#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_DEBUG
	void cpuinfo_log_debug(const char* format, ...);
#else
	static inline void cpuinfo_log_debug(const char* format, ...) { }
#endif

#ifdef __GNUC__
__attribute__((__format__(__printf__, 1, 2)))
#endif
#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_INFO
	void cpuinfo_log_info(const char* format, ...);
#else
	static inline void cpuinfo_log_info(const char* format, ...) { }
#endif

#ifdef __GNUC__
__attribute__((__format__(__printf__, 1, 2)))
#endif
#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_WARNING
	void cpuinfo_log_warning(const char* format, ...);
#else
	static inline void cpuinfo_log_warning(const char* format, ...) { }
#endif

#ifdef __GNUC__
__attribute__((__format__(__printf__, 1, 2)))
#endif
#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_ERROR
	void cpuinfo_log_error(const char* format, ...);
#else
	static inline void cpuinfo_log_error(const char* format, ...) { }
#endif
