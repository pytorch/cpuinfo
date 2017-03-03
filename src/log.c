#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include <log.h>

#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_ERROR
	void cpuinfo_log_error(const char* format, ...) {
		va_list args;
		va_start(args, format);

		dprintf(STDERR_FILENO, "Error: ");
		vdprintf(STDERR_FILENO, format, args);
		dprintf(STDERR_FILENO, "\n");

		va_end(args);
	}
#endif

#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_WARNING
	void cpuinfo_log_warning(const char* format, ...) {
		va_list args;
		va_start(args, format);

		dprintf(STDERR_FILENO, "Warning: ");
		vdprintf(STDERR_FILENO, format, args);
		dprintf(STDERR_FILENO, "\n");

		va_end(args);
	}
#endif

#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_INFO
	void cpuinfo_log_info(const char* format, ...) {
		va_list args;
		va_start(args, format);

		vdprintf(STDOUT_FILENO, format, args);
		dprintf(STDOUT_FILENO, "\n");

		va_end(args);
	}
#endif

#if CPUINFO_LOG_LEVEL >= CPUINFO_LOG_DEBUG
	void cpuinfo_log_debug(const char* format, ...) {
		va_list args;
		va_start(args, format);

		vdprintf(STDOUT_FILENO, format, args);
		dprintf(STDOUT_FILENO, "\n");

		va_end(args);
	}
#endif
