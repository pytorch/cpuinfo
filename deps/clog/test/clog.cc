#include <gtest/gtest.h>

#include <clog.h>

CLOG_DEFINE_LOG_DEBUG(named_, "Unit Test", CLOG_DEBUG);
CLOG_DEFINE_LOG_INFO(named_, "Unit Test", CLOG_INFO);
CLOG_DEFINE_LOG_WARNING(named_, "Unit Test", CLOG_WARNING);
CLOG_DEFINE_LOG_ERROR(named_, "Unit Test", CLOG_ERROR);
CLOG_DEFINE_LOG_FATAL(named_, "Unit Test", CLOG_FATAL);

CLOG_DEFINE_LOG_DEBUG(nameless_, NULL, CLOG_DEBUG);
CLOG_DEFINE_LOG_INFO(nameless_, NULL, CLOG_INFO);
CLOG_DEFINE_LOG_WARNING(nameless_, NULL, CLOG_WARNING);
CLOG_DEFINE_LOG_ERROR(nameless_, NULL, CLOG_ERROR);
CLOG_DEFINE_LOG_FATAL(nameless_, NULL, CLOG_FATAL);

CLOG_DEFINE_LOG_DEBUG(suppressed_, NULL, CLOG_INFO);
CLOG_DEFINE_LOG_INFO(suppressed_, NULL, CLOG_WARNING);
CLOG_DEFINE_LOG_WARNING(suppressed_, NULL, CLOG_ERROR);
CLOG_DEFINE_LOG_ERROR(suppressed_, NULL, CLOG_FATAL);
CLOG_DEFINE_LOG_FATAL(suppressed_, NULL, CLOG_NONE);


TEST(CLOG, debug) {
	named_log_debug("test debug message with a module name");
	nameless_log_debug("test debug message without a module name");
	suppressed_log_debug("test suppressed debug message");
}

TEST(CLOG, info) {
	named_log_info("test info message with a module name");
	nameless_log_info("test info message without a module name");
	suppressed_log_info("test suppressed info message");
}

TEST(CLOG, warning) {
	named_log_warning("test warning message with a module name");
	nameless_log_warning("test warning message without a module name");
	suppressed_log_warning("test suppressed warning message");
}

TEST(CLOG, error) {
	named_log_error("test error message with a module name");
	nameless_log_error("test error message without a module name");
	suppressed_log_error("test suppressed error message");
}
