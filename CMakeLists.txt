CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)

INCLUDE(GNUInstallDirs)

# ---[ Project and semantic versioning.
PROJECT(cpuinfo C CXX)

# ---[ Options.
SET(CPUINFO_LIBRARY_TYPE "default" CACHE STRING "Type of cpuinfo library (shared, static, or default) to build")
SET_PROPERTY(CACHE CPUINFO_LIBRARY_TYPE PROPERTY STRINGS default static shared)
SET(CPUINFO_RUNTIME_TYPE "default" CACHE STRING "Type of runtime library (shared, static, or default) to use")
SET_PROPERTY(CACHE CPUINFO_RUNTIME_TYPE PROPERTY STRINGS default static shared)
SET(CPUINFO_LOG_LEVEL "default" CACHE STRING "Minimum logging level (info with lower severity will be ignored)")
SET_PROPERTY(CACHE CPUINFO_LOG_LEVEL PROPERTY STRINGS default debug info warning error fatal none)
OPTION(CPUINFO_BUILD_TOOLS "Build command-line tools" ON)
OPTION(CPUINFO_BUILD_UNIT_TESTS "Build cpuinfo unit tests" ON)
OPTION(CPUINFO_BUILD_MOCK_TESTS "Build cpuinfo mock tests" ON)
OPTION(CPUINFO_BUILD_BENCHMARKS "Build cpuinfo micro-benchmarks" ON)

# ---[ CMake options
IF(CPUINFO_BUILD_UNIT_TESTS OR CPUINFO_BUILD_MOCK_TESTS)
  ENABLE_TESTING()
ENDIF()

MACRO(CPUINFO_TARGET_ENABLE_C99 target)
  IF(${CMAKE_VERSION} VERSION_LESS "3.1")
    IF(NOT MSVC)
      TARGET_COMPILE_OPTIONS(${target} PRIVATE -std=c99)
    ENDIF()
  ELSE()
    SET_TARGET_PROPERTIES(${target} PROPERTIES
      C_STANDARD 99
      C_EXTENSIONS NO)
  ENDIF()
ENDMACRO()

MACRO(CPUINFO_TARGET_ENABLE_CXX11 target)
  IF(${CMAKE_VERSION} VERSION_LESS "3.1")
    IF(NOT MSVC)
      TARGET_COMPILE_OPTIONS(${target} PRIVATE -std=c++11)
    ENDIF()
  ELSE()
    SET_TARGET_PROPERTIES(${target} PROPERTIES
      CXX_STANDARD 11
      CXX_EXTENSIONS NO)
  ENDIF()
ENDMACRO()

MACRO(CPUINFO_TARGET_RUNTIME_LIBRARY target)
  IF(MSVC AND NOT CPUINFO_RUNTIME_TYPE STREQUAL "default")
    IF(CPUINFO_RUNTIME_TYPE STREQUAL "shared")
      TARGET_COMPILE_OPTIONS(${target} PRIVATE
        "/MD$<$<CONFIG:Debug>:d>")
    ELSEIF(CPUINFO_RUNTIME_TYPE STREQUAL "static")
      TARGET_COMPILE_OPTIONS(${target} PRIVATE
        "/MT$<$<CONFIG:Debug>:d>")
    ENDIF()
  ENDIF()
ENDMACRO()

# ---[ Build flags
SET(CPUINFO_SUPPORTED_PLATFORM TRUE)
IF(NOT CMAKE_SYSTEM_PROCESSOR)
  IF(NOT IOS)
    MESSAGE(WARNING
      "Target processor architecture is not specified. "
      "cpuinfo will compile, but cpuinfo_initialize() will always fail.")
    SET(CPUINFO_SUPPORTED_PLATFORM FALSE)
  ENDIF()
ELSEIF(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|AMD64|x86(_64)?|armv[5-8].*|aarch64)$")
  MESSAGE(WARNING
    "Target processor architecture \"${CMAKE_SYSTEM_PROCESSOR}\" is not supported in cpuinfo. "
    "cpuinfo will compile, but cpuinfo_initialize() will always fail.")
  SET(CPUINFO_SUPPORTED_PLATFORM FALSE)
ENDIF()

IF(NOT CMAKE_SYSTEM_NAME)
    MESSAGE(WARNING
      "Target operating system is not specified. "
      "cpuinfo will compile, but cpuinfo_initialize() will always fail.")
  SET(CPUINFO_SUPPORTED_PLATFORM FALSE)
ELSEIF(NOT CMAKE_SYSTEM_NAME MATCHES "^(Windows|Darwin|Linux|Android)$")
  IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14" AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    MESSAGE(WARNING
      "Target operating system \"${CMAKE_SYSTEM_NAME}\" is not supported in cpuinfo. "
      "cpuinfo will compile, but cpuinfo_initialize() will always fail.")
    SET(CPUINFO_SUPPORTED_PLATFORM FALSE)
  ENDIF()
ENDIF()

# ---[ Download deps
SET(CONFU_DEPENDENCIES_SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps
  CACHE PATH "Confu-style dependencies source directory")
SET(CONFU_DEPENDENCIES_BINARY_DIR ${CMAKE_BINARY_DIR}/deps
  CACHE PATH "Confu-style dependencies binary directory")

IF(CPUINFO_BUILD_MOCK_TESTS OR CPUINFO_BUILD_UNIT_TESTS)
  IF(CPUINFO_SUPPORTED_PLATFORM AND NOT DEFINED GOOGLETEST_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Google Test to ${CONFU_DEPENDENCIES_SOURCE_DIR}/googletest (define GOOGLETEST_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadGoogleTest.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest-download/CMakeLists.txt")
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
      WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest-download")
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
      WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest-download")
    SET(GOOGLETEST_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/googletest" CACHE STRING "Google Test source directory")
  ENDIF()
ENDIF()

IF(CPUINFO_BUILD_BENCHMARKS)
  IF(CPUINFO_SUPPORTED_PLATFORM AND NOT DEFINED GOOGLEBENCHMARK_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Google Benchmark to ${CONFU_DEPENDENCIES_SOURCE_DIR}/googlebenchmark (define GOOGLEBENCHMARK_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadGoogleBenchmark.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/googlebenchmark-download/CMakeLists.txt")
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
      WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/googlebenchmark-download")
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
      WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/googlebenchmark-download")
    SET(GOOGLEBENCHMARK_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/googlebenchmark" CACHE STRING "Google Benchmark source directory")
  ENDIF()
ENDIF()

# ---[ cpuinfo library
SET(CPUINFO_SRCS
  src/init.c
  src/api.c
  src/cache.c)

IF(CPUINFO_SUPPORTED_PLATFORM)
  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|AMD64|x86(_64)?)$" OR IOS_ARCH MATCHES "^(i386|x86_64)$")
    LIST(APPEND CPUINFO_SRCS
      src/x86/init.c
      src/x86/info.c
      src/x86/vendor.c
      src/x86/uarch.c
      src/x86/name.c
      src/x86/topology.c
      src/x86/isa.c
      src/x86/cache/init.c
      src/x86/cache/descriptor.c
      src/x86/cache/deterministic.c)
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
      LIST(APPEND CPUINFO_SRCS
        src/x86/linux/init.c
        src/x86/linux/cpuinfo.c)
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
      LIST(APPEND CPUINFO_SRCS src/x86/mach/init.c)
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
      LIST(APPEND CPUINFO_SRCS src/x86/windows/init.c)
    ENDIF()
  ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv[5-8].*|aarch64)$" OR IOS_ARCH MATCHES "^(armv7.*|arm64.*)$")
    LIST(APPEND CPUINFO_SRCS
      src/arm/uarch.c
      src/arm/cache.c)
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
      LIST(APPEND CPUINFO_SRCS
        src/arm/linux/init.c
        src/arm/linux/cpuinfo.c
        src/arm/linux/clusters.c
        src/arm/linux/chipset.c
        src/arm/linux/midr.c
        src/arm/linux/hwcap.c)
      IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^armv[5-8]")
        LIST(APPEND CPUINFO_SRCS src/arm/linux/aarch32-isa.c)
        IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND ANDROID_ABI STREQUAL "armeabi")
          SET_SOURCE_FILES_PROPERTIES(src/arm/linux/aarch32-isa.c PROPERTIES COMPILE_FLAGS -marm)
        ENDIF()
      ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        LIST(APPEND CPUINFO_SRCS src/arm/linux/aarch64-isa.c)
      ENDIF()
    ELSEIF(IOS)
      LIST(APPEND CPUINFO_SRCS src/arm/mach/init.c)
    ENDIF()
    IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
      LIST(APPEND CPUINFO_SRCS
        src/arm/android/properties.c)
    ENDIF()
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_SRCS
      src/linux/smallfile.c
      src/linux/multiline.c
      src/linux/cpulist.c
      src/linux/processors.c)
  ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    LIST(APPEND CPUINFO_SRCS src/mach/topology.c)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    SET(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
    FIND_PACKAGE(Threads REQUIRED)
  ENDIF()
ENDIF()

IF(CPUINFO_LIBRARY_TYPE STREQUAL "default")
  ADD_LIBRARY(cpuinfo ${CPUINFO_SRCS})
ELSEIF(CPUINFO_LIBRARY_TYPE STREQUAL "shared")
  ADD_LIBRARY(cpuinfo SHARED ${CPUINFO_SRCS})
ELSEIF(CPUINFO_LIBRARY_TYPE STREQUAL "static")
  ADD_LIBRARY(cpuinfo STATIC ${CPUINFO_SRCS})
ELSE()
  MESSAGE(FATAL_ERROR "Unsupported library type ${CPUINFO_LIBRARY_TYPE}")
ENDIF()
ADD_LIBRARY(cpuinfo_internals STATIC ${CPUINFO_SRCS})
CPUINFO_TARGET_ENABLE_C99(cpuinfo)
CPUINFO_TARGET_ENABLE_C99(cpuinfo_internals)
CPUINFO_TARGET_RUNTIME_LIBRARY(cpuinfo)
SET_TARGET_PROPERTIES(cpuinfo PROPERTIES PUBLIC_HEADER include/cpuinfo.h)
TARGET_INCLUDE_DIRECTORIES(cpuinfo BEFORE PUBLIC include)
TARGET_INCLUDE_DIRECTORIES(cpuinfo BEFORE PRIVATE src)
TARGET_INCLUDE_DIRECTORIES(cpuinfo_internals BEFORE PUBLIC include src)
IF(CPUINFO_LOG_LEVEL STREQUAL "default")
  # default logging level: error (subject to change)
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=2)
ELSEIF(CPUINFO_LOG_LEVEL STREQUAL "debug")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=5)
ELSEIF(CPUINFO_LOG_LEVEL STREQUAL "info")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=4)
ELSEIF(CPUINFO_LOG_LEVEL STREQUAL "warning")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=3)
ELSEIF(CPUINFO_LOG_LEVEL STREQUAL "error")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=2)
ELSEIF(CPUINFO_LOG_LEVEL STREQUAL "fatal")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=1)
ELSEIF(CPUINFO_LOG_LEVEL STREQUAL "none")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE CPUINFO_LOG_LEVEL=0)
ELSE()
  MESSAGE(FATAL_ERROR "Unsupported logging level ${CPUINFO_LOG_LEVEL}")
ENDIF()
TARGET_COMPILE_DEFINITIONS(cpuinfo_internals PRIVATE CPUINFO_LOG_LEVEL=0)

IF(CPUINFO_SUPPORTED_PLATFORM)
  TARGET_COMPILE_DEFINITIONS(cpuinfo INTERFACE CPUINFO_SUPPORTED_PLATFORM=1)
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    TARGET_LINK_LIBRARIES(cpuinfo PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    TARGET_LINK_LIBRARIES(cpuinfo_internals PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE _GNU_SOURCE=1)
    TARGET_COMPILE_DEFINITIONS(cpuinfo_internals PRIVATE _GNU_SOURCE=1)
  ENDIF()
ELSE()
  TARGET_COMPILE_DEFINITIONS(cpuinfo INTERFACE CPUINFO_SUPPORTED_PLATFORM=0)
ENDIF()

# ---[ cpuinfo dependencies: clog
IF(NOT DEFINED CLOG_SOURCE_DIR)
  SET(CLOG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps/clog")
ENDIF()
IF(NOT TARGET clog)
  SET(CLOG_BUILD_TESTS OFF CACHE BOOL "")
  SET(CLOG_RUNTIME_TYPE "${CPUINFO_RUNTIME_TYPE}" CACHE STRING "")
  ADD_SUBDIRECTORY(
    "${CLOG_SOURCE_DIR}")
  # We build static version of clog but a dynamic library may indirectly depend on it
  SET_PROPERTY(TARGET clog PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()
TARGET_LINK_LIBRARIES(cpuinfo PRIVATE clog)
TARGET_LINK_LIBRARIES(cpuinfo_internals PRIVATE clog)

INSTALL(TARGETS cpuinfo
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ---[ cpuinfo micro-benchmarks
IF(CPUINFO_SUPPORTED_PLATFORM AND CPUINFO_BUILD_BENCHMARKS)
  # ---[ Build google benchmark
  IF(NOT TARGET benchmark)
    SET(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
    ADD_SUBDIRECTORY(
      "${GOOGLEBENCHMARK_SOURCE_DIR}"
      "${CONFU_DEPENDENCIES_BINARY_DIR}/googlebenchmark")
  ENDIF()

  IF(CMAKE_SYSTEM_NAME MATCHES "^(Linux|Android)$")
    ADD_EXECUTABLE(get-current-bench bench/get-current.cc)
    TARGET_LINK_LIBRARIES(get-current-bench cpuinfo benchmark)
  ENDIF()

  ADD_EXECUTABLE(init-bench bench/init.cc)
  TARGET_LINK_LIBRARIES(init-bench cpuinfo benchmark)
ENDIF()

IF(CPUINFO_SUPPORTED_PLATFORM)
  IF(CPUINFO_BUILD_MOCK_TESTS OR CPUINFO_BUILD_UNIT_TESTS)
    # ---[ Build google test
    IF(NOT TARGET gtest)
      IF(MSVC AND NOT CPUINFO_RUNTIME_TYPE STREQUAL "static")
        SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
      ENDIF()
      ADD_SUBDIRECTORY(
        "${GOOGLETEST_SOURCE_DIR}"
        "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest")
    ENDIF()
  ENDIF()
ENDIF()

# ---[ cpuinfo mock library and mock tests
IF(CPUINFO_SUPPORTED_PLATFORM AND CPUINFO_BUILD_MOCK_TESTS)
  SET(CPUINFO_MOCK_SRCS "${CPUINFO_SRCS}")
  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|AMD64|x86_64)$")
    LIST(APPEND CPUINFO_MOCK_SRCS src/x86/mockcpuid.c)
  ENDIF()
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_MOCK_SRCS src/linux/mockfile.c)
  ENDIF()

  ADD_LIBRARY(cpuinfo_mock STATIC ${CPUINFO_MOCK_SRCS})
  CPUINFO_TARGET_ENABLE_C99(cpuinfo_mock)
  CPUINFO_TARGET_RUNTIME_LIBRARY(cpuinfo_mock)
  SET_TARGET_PROPERTIES(cpuinfo_mock PROPERTIES PUBLIC_HEADER include/cpuinfo.h)
  TARGET_INCLUDE_DIRECTORIES(cpuinfo_mock BEFORE PUBLIC include)
  TARGET_INCLUDE_DIRECTORIES(cpuinfo_mock BEFORE PRIVATE src)
  TARGET_COMPILE_DEFINITIONS(cpuinfo_mock PUBLIC CPUINFO_MOCK=1)
  TARGET_COMPILE_DEFINITIONS(cpuinfo_mock PRIVATE CLOG_LOG_TO_STDIO=1)
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    TARGET_LINK_LIBRARIES(cpuinfo_mock PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    TARGET_COMPILE_DEFINITIONS(cpuinfo_mock PRIVATE _GNU_SOURCE=1)
  ENDIF()
  TARGET_LINK_LIBRARIES(cpuinfo_mock PRIVATE clog)

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a)$")
    ADD_EXECUTABLE(atm7029b-tablet-test test/mock/atm7029b-tablet.cc)
    TARGET_INCLUDE_DIRECTORIES(atm7029b-tablet-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(atm7029b-tablet-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(atm7029b-tablet-test atm7029b-tablet-test)

    ADD_EXECUTABLE(blu-r1-hd-test test/mock/blu-r1-hd.cc)
    TARGET_INCLUDE_DIRECTORIES(blu-r1-hd-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(blu-r1-hd-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(blu-r1-hd-test blu-r1-hd-test)

    ADD_EXECUTABLE(galaxy-a3-2016-eu-test test/mock/galaxy-a3-2016-eu.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-a3-2016-eu-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-a3-2016-eu-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-a3-2016-eu-test galaxy-a3-2016-eu-test)

    ADD_EXECUTABLE(galaxy-a8-2016-duos-test test/mock/galaxy-a8-2016-duos.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-a8-2016-duos-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-a8-2016-duos-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-a8-2016-duos-test galaxy-a8-2016-duos-test)

    ADD_EXECUTABLE(galaxy-grand-prime-value-edition-test test/mock/galaxy-grand-prime-value-edition.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-grand-prime-value-edition-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-grand-prime-value-edition-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-grand-prime-value-edition-test galaxy-grand-prime-value-edition-test)

    ADD_EXECUTABLE(galaxy-j1-2016-test test/mock/galaxy-j1-2016.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j1-2016-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j1-2016-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j1-2016-test galaxy-j1-2016-test)

    ADD_EXECUTABLE(galaxy-j5-test test/mock/galaxy-j5.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j5-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j5-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j5-test galaxy-j5-test)

    ADD_EXECUTABLE(galaxy-j7-prime-test test/mock/galaxy-j7-prime.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j7-prime-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j7-prime-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j7-prime-test galaxy-j7-prime-test)

    ADD_EXECUTABLE(galaxy-j7-tmobile-test test/mock/galaxy-j7-tmobile.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j7-tmobile-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j7-tmobile-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j7-tmobile-test galaxy-j7-tmobile-test)

    ADD_EXECUTABLE(galaxy-j7-uae-test test/mock/galaxy-j7-uae.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j7-uae-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j7-uae-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j7-uae-test galaxy-j7-uae-test)

    ADD_EXECUTABLE(galaxy-s3-us-test test/mock/galaxy-s3-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s3-us-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s3-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s3-us-test galaxy-s3-us-test)

    ADD_EXECUTABLE(galaxy-s4-us-test test/mock/galaxy-s4-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s4-us-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s4-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s4-us-test galaxy-s4-us-test)

    ADD_EXECUTABLE(galaxy-s5-global-test test/mock/galaxy-s5-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s5-global-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s5-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s5-global-test galaxy-s5-global-test)

    ADD_EXECUTABLE(galaxy-s5-us-test test/mock/galaxy-s5-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s5-us-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s5-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s5-us-test galaxy-s5-us-test)

    ADD_EXECUTABLE(galaxy-tab-3-7.0-test test/mock/galaxy-tab-3-7.0.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-tab-3-7.0-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-tab-3-7.0-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-tab-3-7.0-test galaxy-tab-3-7.0-test)

    ADD_EXECUTABLE(galaxy-tab-3-lite-test test/mock/galaxy-tab-3-lite.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-tab-3-lite-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-tab-3-lite-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-tab-3-lite-test galaxy-tab-3-lite-test)

    ADD_EXECUTABLE(galaxy-win-duos-test test/mock/galaxy-win-duos.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-win-duos-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-win-duos-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-win-duos-test galaxy-win-duos-test)

    ADD_EXECUTABLE(huawei-ascend-p7-test test/mock/huawei-ascend-p7.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-ascend-p7-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-ascend-p7-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-ascend-p7-test huawei-ascend-p7-test)

    ADD_EXECUTABLE(huawei-honor-6-test test/mock/huawei-honor-6.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-honor-6-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-honor-6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-honor-6-test huawei-honor-6-test)

    ADD_EXECUTABLE(lenovo-a6600-plus-test test/mock/lenovo-a6600-plus.cc)
    TARGET_INCLUDE_DIRECTORIES(lenovo-a6600-plus-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lenovo-a6600-plus-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lenovo-a6600-plus-test lenovo-a6600-plus-test)

    ADD_EXECUTABLE(lenovo-vibe-x2-test test/mock/lenovo-vibe-x2.cc)
    TARGET_INCLUDE_DIRECTORIES(lenovo-vibe-x2-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lenovo-vibe-x2-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lenovo-vibe-x2-test lenovo-vibe-x2-test)

    ADD_EXECUTABLE(lg-k10-eu-test test/mock/lg-k10-eu.cc)
    TARGET_INCLUDE_DIRECTORIES(lg-k10-eu-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lg-k10-eu-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lg-k10-eu-test lg-k10-eu-test)

    ADD_EXECUTABLE(lg-optimus-g-pro-test test/mock/lg-optimus-g-pro.cc)
    TARGET_INCLUDE_DIRECTORIES(lg-optimus-g-pro-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lg-optimus-g-pro-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lg-optimus-g-pro-test lg-optimus-g-pro-test)

    ADD_EXECUTABLE(moto-e-gen1-test test/mock/moto-e-gen1.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-e-gen1-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-e-gen1-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-e-gen1-test moto-e-gen1-test)

    ADD_EXECUTABLE(moto-g-gen1-test test/mock/moto-g-gen1.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen1-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen1-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen1-test moto-g-gen1-test)

    ADD_EXECUTABLE(moto-g-gen2-test test/mock/moto-g-gen2.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen2-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen2-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen2-test moto-g-gen2-test)

    ADD_EXECUTABLE(moto-g-gen3-test test/mock/moto-g-gen3.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen3-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen3-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen3-test moto-g-gen3-test)

    ADD_EXECUTABLE(moto-g-gen4-test test/mock/moto-g-gen4.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen4-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen4-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen4-test moto-g-gen4-test)

    ADD_EXECUTABLE(moto-g-gen5-test test/mock/moto-g-gen5.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen5-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen5-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen5-test moto-g-gen5-test)

    ADD_EXECUTABLE(nexus-s-test test/mock/nexus-s.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus-s-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus-s-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus-s-test nexus-s-test)

    ADD_EXECUTABLE(nexus4-test test/mock/nexus4.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus4-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus4-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus4-test nexus4-test)

    ADD_EXECUTABLE(nexus6-test test/mock/nexus6.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus6-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus6-test nexus6-test)

    ADD_EXECUTABLE(nexus10-test test/mock/nexus10.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus10-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus10-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus10-test nexus10-test)

    ADD_EXECUTABLE(padcod-10.1-test test/mock/padcod-10.1.cc)
    TARGET_INCLUDE_DIRECTORIES(padcod-10.1-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(padcod-10.1-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(padcod-10.1-test padcod-10.1-test)

    ADD_EXECUTABLE(xiaomi-redmi-2a-test test/mock/xiaomi-redmi-2a.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-redmi-2a-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-redmi-2a-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-redmi-2a-test xiaomi-redmi-2a-test)

    ADD_EXECUTABLE(xperia-sl-test test/mock/xperia-sl.cc)
    TARGET_INCLUDE_DIRECTORIES(xperia-sl-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xperia-sl-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xperia-sl-test xperia-sl-test)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a|aarch64)$")
    ADD_EXECUTABLE(alcatel-revvl-test test/mock/alcatel-revvl.cc)
    TARGET_INCLUDE_DIRECTORIES(alcatel-revvl-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(alcatel-revvl-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(alcatel-revvl-test alcatel-revvl-test)

    ADD_EXECUTABLE(galaxy-a8-2018-test test/mock/galaxy-a8-2018.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-a8-2018-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-a8-2018-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-a8-2018-test galaxy-a8-2018-test)

    ADD_EXECUTABLE(galaxy-c9-pro-test test/mock/galaxy-c9-pro.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-c9-pro-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-c9-pro-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-c9-pro-test galaxy-c9-pro-test)

    ADD_EXECUTABLE(galaxy-s6-test test/mock/galaxy-s6.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s6-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s6-test galaxy-s6-test)

    ADD_EXECUTABLE(galaxy-s7-us-test test/mock/galaxy-s7-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s7-us-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s7-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s7-us-test galaxy-s7-us-test)

    ADD_EXECUTABLE(galaxy-s7-global-test test/mock/galaxy-s7-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s7-global-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s7-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s7-global-test galaxy-s7-global-test)

    ADD_EXECUTABLE(galaxy-s8-us-test test/mock/galaxy-s8-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s8-us-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s8-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s8-us-test galaxy-s8-us-test)

    ADD_EXECUTABLE(galaxy-s8-global-test test/mock/galaxy-s8-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s8-global-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s8-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s8-global-test galaxy-s8-global-test)

    ADD_EXECUTABLE(galaxy-s9-us-test test/mock/galaxy-s9-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s9-us-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s9-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s9-us-test galaxy-s9-us-test)

    ADD_EXECUTABLE(galaxy-s9-global-test test/mock/galaxy-s9-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s9-global-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s9-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s9-global-test galaxy-s9-global-test)

    ADD_EXECUTABLE(huawei-mate-8-test test/mock/huawei-mate-8.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-8-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-8-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-8-test huawei-mate-8-test)

    ADD_EXECUTABLE(huawei-mate-9-test test/mock/huawei-mate-9.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-9-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-9-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-9-test huawei-mate-9-test)

    ADD_EXECUTABLE(huawei-mate-10-test test/mock/huawei-mate-10.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-10-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-10-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-10-test huawei-mate-10-test)

    ADD_EXECUTABLE(huawei-mate-20-test test/mock/huawei-mate-20.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-20-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-20-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-20-test huawei-mate-20-test)

    ADD_EXECUTABLE(huawei-p8-lite-test test/mock/huawei-p8-lite.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-p8-lite-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-p8-lite-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-p8-lite-test huawei-p8-lite-test)

    ADD_EXECUTABLE(huawei-p9-lite-test test/mock/huawei-p9-lite.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-p9-lite-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-p9-lite-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-p9-lite-test huawei-p9-lite-test)

    ADD_EXECUTABLE(huawei-p20-pro-test test/mock/huawei-p20-pro.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-p20-pro-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-p20-pro-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-p20-pro-test huawei-p20-pro-test)

    ADD_EXECUTABLE(iconia-one-10-test test/mock/iconia-one-10.cc)
    TARGET_INCLUDE_DIRECTORIES(iconia-one-10-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(iconia-one-10-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(iconia-one-10-test iconia-one-10-test)

    ADD_EXECUTABLE(meizu-pro-6-test test/mock/meizu-pro-6.cc)
    TARGET_INCLUDE_DIRECTORIES(meizu-pro-6-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(meizu-pro-6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(meizu-pro-6-test meizu-pro-6-test)

    ADD_EXECUTABLE(meizu-pro-6s-test test/mock/meizu-pro-6s.cc)
    TARGET_INCLUDE_DIRECTORIES(meizu-pro-6s-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(meizu-pro-6s-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(meizu-pro-6s-test meizu-pro-6s-test)

    ADD_EXECUTABLE(meizu-pro-7-plus-test test/mock/meizu-pro-7-plus.cc)
    TARGET_INCLUDE_DIRECTORIES(meizu-pro-7-plus-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(meizu-pro-7-plus-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(meizu-pro-7-plus-test meizu-pro-7-plus-test)

    ADD_EXECUTABLE(nexus5x-test test/mock/nexus5x.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus5x-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus5x-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus5x-test nexus5x-test)

    ADD_EXECUTABLE(nexus6p-test test/mock/nexus6p.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus6p-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus6p-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus6p-test nexus6p-test)

    ADD_EXECUTABLE(nexus9-test test/mock/nexus9.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus9-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus9-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus9-test nexus9-test)

    ADD_EXECUTABLE(oneplus-3t-test test/mock/oneplus-3t.cc)
    TARGET_INCLUDE_DIRECTORIES(oneplus-3t-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oneplus-3t-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oneplus-3t-test oneplus-3t-test)

    ADD_EXECUTABLE(oneplus-5-test test/mock/oneplus-5.cc)
    TARGET_INCLUDE_DIRECTORIES(oneplus-5-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oneplus-5-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oneplus-5-test oneplus-5-test)

    ADD_EXECUTABLE(oneplus-5t-test test/mock/oneplus-5t.cc)
    TARGET_INCLUDE_DIRECTORIES(oneplus-5t-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oneplus-5t-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oneplus-5t-test oneplus-5t-test)

    ADD_EXECUTABLE(oppo-a37-test test/mock/oppo-a37.cc)
    TARGET_INCLUDE_DIRECTORIES(oppo-a37-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oppo-a37-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oppo-a37-test oppo-a37-test)

    ADD_EXECUTABLE(oppo-r9-test test/mock/oppo-r9.cc)
    TARGET_INCLUDE_DIRECTORIES(oppo-r9-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oppo-r9-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oppo-r9-test oppo-r9-test)

    ADD_EXECUTABLE(oppo-r15-test test/mock/oppo-r15.cc)
    TARGET_INCLUDE_DIRECTORIES(oppo-r15-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oppo-r15-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oppo-r15-test oppo-r15-test)

    ADD_EXECUTABLE(pixel-test test/mock/pixel.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-test pixel-test)

    ADD_EXECUTABLE(pixel-c-test test/mock/pixel-c.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-c-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-c-test pixel-c-test)

    ADD_EXECUTABLE(pixel-xl-test test/mock/pixel-xl.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-xl-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-xl-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-xl-test pixel-xl-test)

    ADD_EXECUTABLE(pixel-2-xl-test test/mock/pixel-2-xl.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-2-xl-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-2-xl-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-2-xl-test pixel-2-xl-test)

    ADD_EXECUTABLE(xiaomi-mi-5c-test test/mock/xiaomi-mi-5c.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-mi-5c-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-mi-5c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-mi-5c-test xiaomi-mi-5c-test)

    ADD_EXECUTABLE(xiaomi-redmi-note-3-test test/mock/xiaomi-redmi-note-3.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-redmi-note-3-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-redmi-note-3-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-redmi-note-3-test xiaomi-redmi-note-3-test)

    ADD_EXECUTABLE(xiaomi-redmi-note-4-test test/mock/xiaomi-redmi-note-4.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-redmi-note-4-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-redmi-note-4-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-redmi-note-4-test xiaomi-redmi-note-4-test)

    ADD_EXECUTABLE(xperia-c4-dual-test test/mock/xperia-c4-dual.cc)
    TARGET_INCLUDE_DIRECTORIES(xperia-c4-dual-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xperia-c4-dual-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xperia-c4-dual-test xperia-c4-dual-test)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64)$")
    ADD_EXECUTABLE(alldocube-iwork8-test test/mock/alldocube-iwork8.cc)
    TARGET_INCLUDE_DIRECTORIES(alldocube-iwork8-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(alldocube-iwork8-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(alldocube-iwork8-test alldocube-iwork8-test)

    ADD_EXECUTABLE(leagoo-t5c-test test/mock/leagoo-t5c.cc)
    TARGET_INCLUDE_DIRECTORIES(leagoo-t5c-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(leagoo-t5c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(leagoo-t5c-test leagoo-t5c-test)

    ADD_EXECUTABLE(memo-pad-7-test test/mock/memo-pad-7.cc)
    TARGET_INCLUDE_DIRECTORIES(memo-pad-7-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(memo-pad-7-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(memo-pad-7-test memo-pad-7-test)

    ADD_EXECUTABLE(zenfone-c-test test/mock/zenfone-c.cc)
    TARGET_INCLUDE_DIRECTORIES(zenfone-c-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(zenfone-c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(zenfone-c-test zenfone-c-test)

    ADD_EXECUTABLE(zenfone-2-test test/mock/zenfone-2.cc)
    TARGET_INCLUDE_DIRECTORIES(zenfone-2-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(zenfone-2-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(zenfone-2-test zenfone-2-test)

    ADD_EXECUTABLE(zenfone-2e-test test/mock/zenfone-2e.cc)
    TARGET_INCLUDE_DIRECTORIES(zenfone-2e-test BEFORE PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(zenfone-2e-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(zenfone-2e-test zenfone-2e-test)
  ENDIF()
ENDIF()

# ---[ cpuinfo unit tests
IF(CPUINFO_SUPPORTED_PLATFORM AND CPUINFO_BUILD_UNIT_TESTS)
  ADD_EXECUTABLE(init-test test/init.cc)
  CPUINFO_TARGET_ENABLE_CXX11(init-test)
  CPUINFO_TARGET_RUNTIME_LIBRARY(init-test)
  TARGET_LINK_LIBRARIES(init-test PRIVATE cpuinfo gtest gtest_main)
  ADD_TEST(init-test init-test)

  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    ADD_EXECUTABLE(get-current-test test/get-current.cc)
    CPUINFO_TARGET_ENABLE_CXX11(get-current-test)
    CPUINFO_TARGET_RUNTIME_LIBRARY(get-current-test)
    TARGET_LINK_LIBRARIES(get-current-test PRIVATE cpuinfo gtest gtest_main)
    ADD_TEST(get-current-test get-current-test)
  ENDIF()

  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|AMD64|x86_64)$")
    ADD_EXECUTABLE(brand-string-test test/name/brand-string.cc)
    CPUINFO_TARGET_ENABLE_CXX11(brand-string-test)
    CPUINFO_TARGET_RUNTIME_LIBRARY(brand-string-test)
    TARGET_LINK_LIBRARIES(brand-string-test PRIVATE cpuinfo_internals gtest gtest_main)
    ADD_TEST(brand-string-test brand-string-test)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv[5-8].*|aarch64)$")
    ADD_LIBRARY(android_properties_interface STATIC test/name/android-properties-interface.c)
    CPUINFO_TARGET_ENABLE_C99(android_properties_interface)
    CPUINFO_TARGET_RUNTIME_LIBRARY(android_properties_interface)
    TARGET_LINK_LIBRARIES(android_properties_interface PRIVATE cpuinfo_internals)

    ADD_EXECUTABLE(chipset-test
      test/name/proc-cpuinfo-hardware.cc
      test/name/ro-product-board.cc
      test/name/ro-board-platform.cc
      test/name/ro-mediatek-platform.cc
      test/name/ro-arch.cc
      test/name/ro-chipname.cc
      test/name/android-properties.cc)
    CPUINFO_TARGET_ENABLE_CXX11(chipset-test)
    CPUINFO_TARGET_RUNTIME_LIBRARY(chipset-test)
    TARGET_LINK_LIBRARIES(chipset-test PRIVATE android_properties_interface gtest gtest_main)
    ADD_TEST(chipset-test chipset-test)

    ADD_EXECUTABLE(cache-test test/arm-cache.cc)
    CPUINFO_TARGET_ENABLE_CXX11(cache-test)
    CPUINFO_TARGET_RUNTIME_LIBRARY(cache-test)
    TARGET_COMPILE_DEFINITIONS(cache-test PRIVATE __STDC_LIMIT_MACROS=1 __STDC_CONSTANT_MACROS=1)
    TARGET_LINK_LIBRARIES(cache-test PRIVATE cpuinfo_internals gtest gtest_main)
    ADD_TEST(cache-test, cache-test)
  ENDIF()
ENDIF()

# ---[ Helper and debug tools
IF(CPUINFO_SUPPORTED_PLATFORM AND CPUINFO_BUILD_TOOLS)
  ADD_EXECUTABLE(isa-info tools/isa-info.c)
  CPUINFO_TARGET_ENABLE_C99(isa-info)
  CPUINFO_TARGET_RUNTIME_LIBRARY(isa-info)
  TARGET_LINK_LIBRARIES(isa-info PRIVATE cpuinfo)
  INSTALL(TARGETS isa-info RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  ADD_EXECUTABLE(cpu-info tools/cpu-info.c)
  CPUINFO_TARGET_ENABLE_C99(cpu-info)
  CPUINFO_TARGET_RUNTIME_LIBRARY(cpu-info)
  TARGET_LINK_LIBRARIES(cpu-info PRIVATE cpuinfo)
  INSTALL(TARGETS cpu-info RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  ADD_EXECUTABLE(cache-info tools/cache-info.c)
  CPUINFO_TARGET_ENABLE_C99(cache-info)
  CPUINFO_TARGET_RUNTIME_LIBRARY(cache-info)
  TARGET_LINK_LIBRARIES(cache-info PRIVATE cpuinfo)
  INSTALL(TARGETS cache-info RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  IF(CMAKE_SYSTEM_NAME MATCHES "^(Android|Linux)$" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv[5-8].*|aarch64)$")
    ADD_EXECUTABLE(auxv-dump tools/auxv-dump.c)
    CPUINFO_TARGET_ENABLE_C99(auxv-dump)
    CPUINFO_TARGET_RUNTIME_LIBRARY(auxv-dump)
    TARGET_LINK_LIBRARIES(auxv-dump PRIVATE ${CMAKE_DL_LIBS} cpuinfo)

    ADD_EXECUTABLE(cpuinfo-dump tools/cpuinfo-dump.c)
    CPUINFO_TARGET_ENABLE_C99(cpuinfo-dump)
    CPUINFO_TARGET_RUNTIME_LIBRARY(cpuinfo-dump)
  ENDIF()

  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|AMD64|x86_64)$")
    ADD_EXECUTABLE(cpuid-dump tools/cpuid-dump.c)
    CPUINFO_TARGET_ENABLE_C99(cpuid-dump)
    CPUINFO_TARGET_RUNTIME_LIBRARY(cpuid-dump)
    TARGET_INCLUDE_DIRECTORIES(cpuid-dump BEFORE PRIVATE src)
    TARGET_INCLUDE_DIRECTORIES(cpuid-dump BEFORE PRIVATE include)
    INSTALL(TARGETS cpuid-dump RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
  ENDIF()
ENDIF()
