#pragma once

#include <stdarg.h>
#include <stdlib.h>
#include <inttypes.h>

#define CLOG_NONE 0
#define CLOG_FATAL 1
#define CLOG_ERROR 2
#define CLOG_WARNING 3
#define CLOG_INFO 4
#define CLOG_DEBUG 5

#ifndef CLOG_VISIBILITY
	#if defined(__ELF__)
		#define CLOG_VISIBILITY __attribute__((__visibility__("internal")))
	#elif defined(__MACH__)
		#define CLOG_VISIBILITY __attribute__((__visibility__("hidden")))
	#else
		#define CLOG_VISIBILITY
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

CLOG_VISIBILITY void clog_vlog_debug(const char* module, const char* format, va_list args);
CLOG_VISIBILITY void clog_vlog_info(const char* module, const char* format, va_list args);
CLOG_VISIBILITY void clog_vlog_warning(const char* module, const char* format, va_list args);
CLOG_VISIBILITY void clog_vlog_error(const char* module, const char* format, va_list args);
CLOG_VISIBILITY void clog_vlog_fatal(const char* module, const char* format, va_list args);

#ifdef __GNUC__
	#define CLOG_DEFINE_LOG_DEBUG(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_debug(const char* format, ...) { \
			if (level >= CLOG_DEBUG) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_debug(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_INFO(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_info(const char* format, ...) { \
			if (level >= CLOG_INFO) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_info(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_WARNING(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_warning(const char* format, ...) { \
			if (level >= CLOG_WARNING) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_warning(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_ERROR(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_error(const char* format, ...) { \
			if (level >= CLOG_ERROR) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_error(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_FATAL(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_fatal(const char* format, ...) { \
			if (level >= CLOG_FATAL) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_fatal(module, format, args); \
				va_end(args); \
			} \
			abort(); \
		}
#else
	#define CLOG_DEFINE_LOG_DEBUG(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_debug(const char* format, ...) { \
			if (level >= CLOG_DEBUG) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_debug(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_INFO(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_info(const char* format, ...) { \
			if (level >= CLOG_INFO) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_info(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_WARNING(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_warning(const char* format, ...) { \
			if (level >= CLOG_WARNING) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_warning(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_ERROR(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_error(const char* format, ...) { \
			if (level >= CLOG_ERROR) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_error(module, format, args); \
				va_end(args); \
			} \
		}

	#define CLOG_DEFINE_LOG_FATAL(prefix, module, level) \
		__attribute__((__format__(__printf__, 1, 2))) \
		inline static void prefix ## log_fatal(const char* format, ...) { \
			if (level >= CLOG_FATAL) { \
				va_list args; \
				va_start(args, format); \
				clog_vlog_fatal(module, format, args); \
				va_end(args); \
			} \
			abort(); \
		}
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
