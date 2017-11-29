CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)

INCLUDE(GNUInstallDirs)

# ---[ Project and semantic versioning.
PROJECT(cpuinfo C CXX)

# ---[ Options.
OPTION(CPUINFO_BUILD_TOOLS "Build command-line tools" ON)
OPTION(CPUINFO_BUILD_UNIT_TESTS "Build C++ unit tests" ON)
OPTION(CPUINFO_BUILD_MOCK_TESTS "Build C++ mock tests" ON)

# ---[ CMake options
IF(CPUINFO_BUILD_UNIT_TESTS OR CPUINFO_BUILD_MOCK_TESTS)
  ENABLE_TESTING()
ENDIF()

# ---[ Build flags
IF(NOT MSVC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

IF(NOT CMAKE_SYSTEM_PROCESSOR)
  MESSAGE(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR not defined")
ELSEIF(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64|armv5te|armv7|armv7f|armv7s|armv7k|armv7-a|armv7l|arm64|aarch64)$")
  MESSAGE(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
ENDIF()

IF(NOT CMAKE_SYSTEM_NAME)
  MESSAGE(FATAL_ERROR "CMAKE_SYSTEM_NAME not defined")
ELSEIF(NOT CMAKE_SYSTEM_NAME MATCHES "^(Darwin|Linux|Android)$")
  MESSAGE(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
ENDIF()

# ---[ Download deps
SET(CONFU_DEPENDENCIES_SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps
  CACHE PATH "Confu-style dependencies source directory")
SET(CONFU_DEPENDENCIES_BINARY_DIR ${CMAKE_BINARY_DIR}/deps
  CACHE PATH "Confu-style dependencies binary directory")

IF(NOT TARGET gtest)
  CONFIGURE_FILE(cmake/DownloadGoogleTest.cmake "${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt")
  EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
  EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
ENDIF()

# ---[ cpuinfo library
SET(CPUINFO_SRCS
  src/init.c
  src/api.c
  src/log.c)

IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64)$")
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
  ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    LIST(APPEND CPUINFO_SRCS src/x86/mach/init.c)
  ENDIF()
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7|armv7f|armv7s|armv7k|armv7-a|armv7l|arm64|aarch64)$")
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
    IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7l|armv7-a)$")
      LIST(APPEND CPUINFO_SRCS src/arm/linux/aarch32-isa.c)
      IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND ANDROID_ABI STREQUAL "armeabi")
        SET_SOURCE_FILES_PROPERTIES(src/arm/linux/aarch32-isa.c PROPERTIES COMPILE_FLAGS -marm)
      ENDIF()
    ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
      LIST(APPEND CPUINFO_SRCS src/arm/linux/aarch64-isa.c)
    ENDIF()
  ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    LIST(APPEND CPUINFO_SRCS
      src/arm/mach/init.c
      src/gpu/gles-ios.m)
    SET_SOURCE_FILES_PROPERTIES(src/gpu/gles-ios.m PROPERTIES COMPILE_FLAGS -fobjc-arc)
  ENDIF()
  IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_SRCS
      src/arm/Android/properties.c)
  ENDIF()
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
  LIST(APPEND CPUINFO_SRCS  
    src/linux/smallfile.c    
    src/linux/multiline.c
    src/linux/current.c
    src/linux/cpulist.c
    src/linux/processors.c)
  IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_SRCS src/gpu/gles2.c)
  ENDIF()
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  LIST(APPEND CPUINFO_SRCS src/mach/topology.c)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
  SET(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
  FIND_PACKAGE(Threads REQUIRED)
ENDIF()

ADD_LIBRARY(cpuinfo ${CPUINFO_SRCS})
SET_TARGET_PROPERTIES(cpuinfo PROPERTIES PUBLIC_HEADER include/cpuinfo.h)
TARGET_INCLUDE_DIRECTORIES(cpuinfo PUBLIC include)
TARGET_INCLUDE_DIRECTORIES(cpuinfo PRIVATE src)
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
  TARGET_LINK_LIBRARIES(cpuinfo PUBLIC ${CMAKE_THREAD_LIBS_INIT})
ENDIF()
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  TARGET_COMPILE_DEFINITIONS(cpuinfo PRIVATE -D_GNU_SOURCE)
ENDIF()
IF(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv7|armv7f|armv7s|armv7k|arm64)$")
  TARGET_LINK_LIBRARIES(cpuinfo INTERFACE "-framework OpenGLES")
  TARGET_LINK_LIBRARIES(cpuinfo INTERFACE "-framework Foundation")
ENDIF()

INSTALL(TARGETS cpuinfo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

IF(CPUINFO_BUILD_MOCK_TESTS OR CPUINFO_BUILD_UNIT_TESTS)
  # ---[ Build google test
  IF(NOT TARGET gtest)
    SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    ADD_SUBDIRECTORY(
      "${CONFU_DEPENDENCIES_SOURCE_DIR}/googletest"
      "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest")
  ENDIF()
ENDIF()

# ---[ cpuinfo mock library and mock tests
IF(CPUINFO_BUILD_MOCK_TESTS)
  SET(CPUINFO_MOCK_SRCS "${CPUINFO_SRCS}")
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_MOCK_SRCS src/linux/mockfile.c)
  ENDIF()

  ADD_LIBRARY(cpuinfo_mock ${CPUINFO_MOCK_SRCS})
  SET_TARGET_PROPERTIES(cpuinfo_mock PROPERTIES PUBLIC_HEADER include/cpuinfo.h)
  TARGET_INCLUDE_DIRECTORIES(cpuinfo_mock PUBLIC include)
  TARGET_INCLUDE_DIRECTORIES(cpuinfo_mock PRIVATE src)
  TARGET_COMPILE_DEFINITIONS(cpuinfo_mock PUBLIC "-DCPUINFO_MOCK=1")
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    TARGET_LINK_LIBRARIES(cpuinfo_mock PUBLIC ${CMAKE_THREAD_LIBS_INIT})
  ENDIF()
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    TARGET_COMPILE_DEFINITIONS(cpuinfo_mock PRIVATE -D_GNU_SOURCE)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a)$")
    ADD_EXECUTABLE(atm7029b-tablet-test test/mock/atm7029b-tablet.cc)
    TARGET_INCLUDE_DIRECTORIES(atm7029b-tablet-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(atm7029b-tablet-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(atm7029b-tablet-test atm7029b-tablet-test)

    ADD_EXECUTABLE(blu-r1-hd-test test/mock/blu-r1-hd.cc)
    TARGET_INCLUDE_DIRECTORIES(blu-r1-hd-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(blu-r1-hd-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(blu-r1-hd-test blu-r1-hd-test)

    ADD_EXECUTABLE(galaxy-a8-2016-duos-test test/mock/galaxy-a8-2016-duos.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-a8-2016-duos-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-a8-2016-duos-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-a8-2016-duos-test galaxy-a8-2016-duos-test)

    ADD_EXECUTABLE(galaxy-grand-prime-value-edition-test test/mock/galaxy-grand-prime-value-edition.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-grand-prime-value-edition-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-grand-prime-value-edition-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-grand-prime-value-edition-test galaxy-grand-prime-value-edition-test)

    ADD_EXECUTABLE(galaxy-s3-us-test test/mock/galaxy-s3-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s3-us-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s3-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s3-us-test galaxy-s3-us-test)

    ADD_EXECUTABLE(galaxy-s4-us-test test/mock/galaxy-s4-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s4-us-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s4-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s4-us-test galaxy-s4-us-test)

    ADD_EXECUTABLE(galaxy-s5-global-test test/mock/galaxy-s5-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s5-global-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s5-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s5-global-test galaxy-s5-global-test)

    ADD_EXECUTABLE(galaxy-s5-us-test test/mock/galaxy-s5-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s5-us-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s5-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s5-us-test galaxy-s5-us-test)

    ADD_EXECUTABLE(galaxy-tab-3-7.0-test test/mock/galaxy-tab-3-7.0.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-tab-3-7.0-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-tab-3-7.0-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-tab-3-7.0-test galaxy-tab-3-7.0-test)

    ADD_EXECUTABLE(galaxy-tab-3-lite-test test/mock/galaxy-tab-3-lite.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-tab-3-lite-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-tab-3-lite-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-tab-3-lite-test galaxy-tab-3-lite-test)

    ADD_EXECUTABLE(galaxy-win-duos-test test/mock/galaxy-win-duos.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-win-duos-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-win-duos-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-win-duos-test galaxy-win-duos-test)

    ADD_EXECUTABLE(huawei-ascend-p7-test test/mock/huawei-ascend-p7.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-ascend-p7-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-ascend-p7-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-ascend-p7-test huawei-ascend-p7-test)

    ADD_EXECUTABLE(lenovo-a6600-plus-test test/mock/lenovo-a6600-plus.cc)
    TARGET_INCLUDE_DIRECTORIES(lenovo-a6600-plus-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lenovo-a6600-plus-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lenovo-a6600-plus-test lenovo-a6600-plus-test)

    ADD_EXECUTABLE(lenovo-vibe-x2-test test/mock/lenovo-vibe-x2.cc)
    TARGET_INCLUDE_DIRECTORIES(lenovo-vibe-x2-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lenovo-vibe-x2-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lenovo-vibe-x2-test lenovo-vibe-x2-test)

    ADD_EXECUTABLE(lg-k10-eu-test test/mock/lg-k10-eu.cc)
    TARGET_INCLUDE_DIRECTORIES(lg-k10-eu-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(lg-k10-eu-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(lg-k10-eu-test lg-k10-eu-test)

    ADD_EXECUTABLE(moto-e-gen1-test test/mock/moto-e-gen1.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-e-gen1-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-e-gen1-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-e-gen1-test moto-e-gen1-test)

    ADD_EXECUTABLE(moto-g-gen2-test test/mock/moto-g-gen2.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen2-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen2-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen2-test moto-g-gen2-test)

    ADD_EXECUTABLE(moto-g-gen3-test test/mock/moto-g-gen3.cc)
    TARGET_INCLUDE_DIRECTORIES(moto-g-gen3-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(moto-g-gen3-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(moto-g-gen3-test moto-g-gen3-test)

    ADD_EXECUTABLE(nexus-s-test test/mock/nexus-s.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus-s-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus-s-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus-s-test nexus-s-test)

    ADD_EXECUTABLE(nexus4-test test/mock/nexus4.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus4-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus4-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus4-test nexus4-test)

    ADD_EXECUTABLE(nexus6-test test/mock/nexus6.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus6-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus6-test nexus6-test)

    ADD_EXECUTABLE(padcod-10.1-test test/mock/padcod-10.1.cc)
    TARGET_INCLUDE_DIRECTORIES(padcod-10.1-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(padcod-10.1-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(padcod-10.1-test padcod-10.1-test)

    ADD_EXECUTABLE(xiaomi-redmi-2a-test test/mock/xiaomi-redmi-2a.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-redmi-2a-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-redmi-2a-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-redmi-2a-test xiaomi-redmi-2a-test)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a|aarch64)$")
    ADD_EXECUTABLE(galaxy-c9-pro-test test/mock/galaxy-c9-pro.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-c9-pro-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-c9-pro-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-c9-pro-test galaxy-c9-pro-test)

    ADD_EXECUTABLE(galaxy-j7-tmobile-test test/mock/galaxy-j7-tmobile.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j7-tmobile-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j7-tmobile-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j7-tmobile-test galaxy-j7-tmobile-test)

    ADD_EXECUTABLE(galaxy-j7-uae-test test/mock/galaxy-j7-uae.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-j7-uae-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-j7-uae-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-j7-uae-test galaxy-j7-uae-test)

    ADD_EXECUTABLE(galaxy-s6-test test/mock/galaxy-s6.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s6-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s6-test galaxy-s6-test)

    ADD_EXECUTABLE(galaxy-s7-us-test test/mock/galaxy-s7-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s7-us-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s7-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s7-us-test galaxy-s7-us-test)

    ADD_EXECUTABLE(galaxy-s7-global-test test/mock/galaxy-s7-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s7-global-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s7-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s7-global-test galaxy-s7-global-test)

    ADD_EXECUTABLE(galaxy-s8-us-test test/mock/galaxy-s8-us.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s8-us-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s8-us-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s8-us-test galaxy-s8-us-test)

    ADD_EXECUTABLE(galaxy-s8-global-test test/mock/galaxy-s8-global.cc)
    TARGET_INCLUDE_DIRECTORIES(galaxy-s8-global-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(galaxy-s8-global-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(galaxy-s8-global-test galaxy-s8-global-test)

    ADD_EXECUTABLE(huawei-p9-lite-test test/mock/huawei-p9-lite.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-p9-lite-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-p9-lite-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-p9-lite-test huawei-p9-lite-test)

    ADD_EXECUTABLE(huawei-mate-8-test test/mock/huawei-mate-8.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-8-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-8-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-8-test huawei-mate-8-test)

    ADD_EXECUTABLE(huawei-mate-9-test test/mock/huawei-mate-9.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-9-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-9-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-9-test huawei-mate-9-test)

    ADD_EXECUTABLE(huawei-mate-10-test test/mock/huawei-mate-10.cc)
    TARGET_INCLUDE_DIRECTORIES(huawei-mate-10-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(huawei-mate-10-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(huawei-mate-10-test huawei-mate-10-test)

    ADD_EXECUTABLE(meizu-pro-6-test test/mock/meizu-pro-6.cc)
    TARGET_INCLUDE_DIRECTORIES(meizu-pro-6-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(meizu-pro-6-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(meizu-pro-6-test meizu-pro-6-test)

    ADD_EXECUTABLE(meizu-pro-6s-test test/mock/meizu-pro-6s.cc)
    TARGET_INCLUDE_DIRECTORIES(meizu-pro-6s-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(meizu-pro-6s-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(meizu-pro-6s-test meizu-pro-6s-test)

    ADD_EXECUTABLE(meizu-pro-7-plus-test test/mock/meizu-pro-7-plus.cc)
    TARGET_INCLUDE_DIRECTORIES(meizu-pro-7-plus-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(meizu-pro-7-plus-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(meizu-pro-7-plus-test meizu-pro-7-plus-test)

    ADD_EXECUTABLE(nexus5x-test test/mock/nexus5x.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus5x-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus5x-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus5x-test nexus5x-test)

    ADD_EXECUTABLE(nexus6p-test test/mock/nexus6p.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus6p-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus6p-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus6p-test nexus6p-test)

    ADD_EXECUTABLE(nexus9-test test/mock/nexus9.cc)
    TARGET_INCLUDE_DIRECTORIES(nexus9-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(nexus9-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(nexus9-test nexus9-test)

    ADD_EXECUTABLE(oppo-r9-test test/mock/oppo-r9.cc)
    TARGET_INCLUDE_DIRECTORIES(oppo-r9-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(oppo-r9-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(oppo-r9-test oppo-r9-test)

    ADD_EXECUTABLE(pixel-test test/mock/pixel.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-test pixel-test)

    ADD_EXECUTABLE(pixel-c-test test/mock/pixel-c.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-c-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-c-test pixel-c-test)

    ADD_EXECUTABLE(pixel-xl-test test/mock/pixel-xl.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-xl-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-xl-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-xl-test pixel-xl-test)

    ADD_EXECUTABLE(pixel-2-xl-test test/mock/pixel-2-xl.cc)
    TARGET_INCLUDE_DIRECTORIES(pixel-2-xl-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(pixel-2-xl-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(pixel-2-xl-test pixel-2-xl-test)

    ADD_EXECUTABLE(xiaomi-mi-5c-test test/mock/xiaomi-mi-5c.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-mi-5c-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-mi-5c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-mi-5c-test xiaomi-mi-5c-test)

    ADD_EXECUTABLE(xiaomi-redmi-note-3-test test/mock/xiaomi-redmi-note-3.cc)
    TARGET_INCLUDE_DIRECTORIES(xiaomi-redmi-note-3-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xiaomi-redmi-note-3-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xiaomi-redmi-note-3-test xiaomi-redmi-note-3-test)

    ADD_EXECUTABLE(xperia-c4-dual-test test/mock/xperia-c4-dual.cc)
    TARGET_INCLUDE_DIRECTORIES(xperia-c4-dual-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(xperia-c4-dual-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(xperia-c4-dual-test xperia-c4-dual-test)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64)$")
    ADD_EXECUTABLE(memo-pad-7-test test/mock/memo-pad-7.cc)
    TARGET_INCLUDE_DIRECTORIES(memo-pad-7-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(memo-pad-7-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(memo-pad-7-test memo-pad-7-test)

    ADD_EXECUTABLE(zenfone-c-test test/mock/zenfone-c.cc)
    TARGET_INCLUDE_DIRECTORIES(zenfone-c-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(zenfone-c-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(zenfone-c-test zenfone-c-test)

    ADD_EXECUTABLE(zenfone-2-test test/mock/zenfone-2.cc)
    TARGET_INCLUDE_DIRECTORIES(zenfone-2-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(zenfone-2-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(zenfone-2-test zenfone-2-test)

    ADD_EXECUTABLE(zenfone-2e-test test/mock/zenfone-2e.cc)
    TARGET_INCLUDE_DIRECTORIES(zenfone-2e-test PRIVATE test/mock)
    TARGET_LINK_LIBRARIES(zenfone-2e-test PRIVATE cpuinfo_mock gtest)
    ADD_TEST(zenfone-2e-test zenfone-2e-test)
  ENDIF()
ENDIF()

# ---[ cpuinfo unit tests
IF(CPUINFO_BUILD_UNIT_TESTS)
  ADD_EXECUTABLE(init-test test/init.cc)
  TARGET_LINK_LIBRARIES(init-test PRIVATE cpuinfo gtest)
  ADD_TEST(init-test init-test)

  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    ADD_EXECUTABLE(get-current-test test/get-current.cc)
    TARGET_LINK_LIBRARIES(get-current-test PRIVATE cpuinfo gtest)
    ADD_TEST(get-current-test get-current-test)
  ENDIF()

  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64)$")
    ADD_EXECUTABLE(brand-string-test test/name/brand-string.cc)
    TARGET_LINK_LIBRARIES(brand-string-test PRIVATE cpuinfo gtest gtest_main)
    ADD_TEST(brand-string-test brand-string-test)
  ENDIF()

  IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a|aarch64)$")
    ADD_LIBRARY(android_properties_interface test/name/android-properties-interface.c)
    TARGET_INCLUDE_DIRECTORIES(android_properties_interface PRIVATE src)
    TARGET_LINK_LIBRARIES(android_properties_interface PRIVATE cpuinfo)

    ADD_EXECUTABLE(chipset-test
      test/name/proc-cpuinfo-hardware.cc
      test/name/ro-product-board.cc
      test/name/ro-board-platform.cc
      test/name/ro-mediatek-platform.cc
      test/name/ro-chipname.cc
      test/name/android-properties.cc)
    TARGET_LINK_LIBRARIES(chipset-test PRIVATE android_properties_interface gtest gtest_main)
    ADD_TEST(chipset-test chipset-test)

    ADD_EXECUTABLE(cache-test test/arm-cache.cc)
    TARGET_COMPILE_DEFINITIONS(cache-test PRIVATE -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)
    TARGET_INCLUDE_DIRECTORIES(cache-test PRIVATE src)
    TARGET_LINK_LIBRARIES(cache-test PRIVATE cpuinfo gtest gtest_main)
    ADD_TEST(cache-test, cache-test)
  ENDIF()
ENDIF()

# ---[ Helper and debug tools
IF(CPUINFO_BUILD_TOOLS)
  ADD_EXECUTABLE(isa-info tools/isa-info.c)
  TARGET_LINK_LIBRARIES(isa-info PRIVATE cpuinfo)

  ADD_EXECUTABLE(cpu-info tools/cpu-info.c)
  TARGET_LINK_LIBRARIES(cpu-info PRIVATE cpuinfo)

  ADD_EXECUTABLE(cache-info tools/cache-info.c)
  TARGET_LINK_LIBRARIES(cache-info PRIVATE cpuinfo)

  IF(CMAKE_SYSTEM_NAME MATCHES "^(Android|Linux)$" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7|armv7-a|armv7l|arm64|aarch64)$")
    ADD_EXECUTABLE(auxv-dump tools/auxv-dump.c)
    TARGET_INCLUDE_DIRECTORIES(auxv-dump PRIVATE src)
    TARGET_INCLUDE_DIRECTORIES(auxv-dump PRIVATE include)
  ENDIF()

  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64)$")
    ADD_EXECUTABLE(cpuid-dump tools/cpuid-dump.c)
    TARGET_INCLUDE_DIRECTORIES(cpuid-dump PRIVATE src)
    TARGET_INCLUDE_DIRECTORIES(cpuid-dump PRIVATE include)
  ENDIF()
ENDIF()
