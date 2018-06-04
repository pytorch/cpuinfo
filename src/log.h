#pragma once

#include <inttypes.h>

#include <clog.h>

#define CPUINFO_LOG_DEBUG_PARSERS 0

#ifndef CPUINFO_LOG_LEVEL
	#define CPUINFO_LOG_LEVEL CLOG_ERROR
#endif

CLOG_DEFINE_LOG_DEBUG(cpuinfo_, "cpuinfo", CPUINFO_LOG_LEVEL);
CLOG_DEFINE_LOG_INFO(cpuinfo_, "cpuinfo", CPUINFO_LOG_LEVEL);
CLOG_DEFINE_LOG_WARNING(cpuinfo_, "cpuinfo", CPUINFO_LOG_LEVEL);
CLOG_DEFINE_LOG_ERROR(cpuinfo_, "cpuinfo", CPUINFO_LOG_LEVEL);
CLOG_DEFINE_LOG_FATAL(cpuinfo_, "cpuinfo", CPUINFO_LOG_LEVEL);
