CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)

INCLUDE(GNUInstallDirs)

# ---[ Project and semantic versioning.
PROJECT(cpuinfo LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "CPU INFOrmation library")

# ---[ Options.
OPTION(BUILD_TOOLS "Build command-line tools" ON)
OPTION(BUILD_UNIT_TESTS "Build C++ unit tests (need googletest)" ON)
OPTION(BUILD_MOCK_TESTS "Build C++ mock tests (need googletest)" ON)

# ---[ Build flags
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
IF(NOT MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -fPIC")
ENDIF()

IF(NOT CMAKE_SYSTEM_PROCESSOR)
  MESSAGE(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR not defined")
ELSEIF(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64|armv5te|armv7-a|aarch64)$")
  MESSAGE(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
ENDIF()

IF(NOT CMAKE_SYSTEM_NAME)
  MESSAGE(FATAL_ERROR "CMAKE_SYSTEM_NAME not defined")
ELSEIF(NOT CMAKE_SYSTEM_NAME MATCHES "^(Darwin|Linux|Android)$")
  MESSAGE(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

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
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a|aarch64)$")
  LIST(APPEND CPUINFO_SRCS
    src/arm/uarch.c
    src/arm/cache.c)
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_SRCS
      src/arm/linux/init.c
      src/arm/linux/cpuinfo.c
      src/arm/linux/clusters.c
      src/arm/linux/midr.c)
  ENDIF()
  IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_SRCS
      src/arm/Android/chipset.c
      src/arm/Android/properties.c)
  ENDIF()
  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv5te|armv7-a)$")
    LIST(APPEND CPUINFO_SRCS src/arm/linux/arm32-isa.c)
    IF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND ANDROID_ABI STREQUAL "armeabi")
      SET_SOURCE_FILES_PROPERTIES(src/arm/linux/arm32-isa.c PROPERTIES COMPILE_FLAGS -marm)
    ENDIF()
  ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    LIST(APPEND CPUINFO_SRCS src/arm/linux/arm64-isa.c)
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

ADD_LIBRARY(cpuinfo ${CPUINFO_SRCS})
SET_TARGET_PROPERTIES(cpuinfo PROPERTIES VERSION ${PROJECT_VERSION})
SET_TARGET_PROPERTIES(cpuinfo PROPERTIES PUBLIC_HEADER include/cpuinfo.h)
TARGET_INCLUDE_DIRECTORIES(cpuinfo PRIVATE include)
TARGET_INCLUDE_DIRECTORIES(cpuinfo PRIVATE src)

INSTALL(TARGETS cpuinfo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ---[ cpuinfo mock library
IF(BUILD_MOCK_TESTS)
  SET(CPUINFO_MOCK_SRCS "${CPUINFO_SRCS}")
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    LIST(APPEND CPUINFO_MOCK_SRCS src/linux/mockfile.c)
  ENDIF()

  ADD_LIBRARY(cpuinfo_mock ${CPUINFO_MOCK_SRCS})
  SET_TARGET_PROPERTIES(cpuinfo_mock PROPERTIES VERSION ${PROJECT_VERSION})
  SET_TARGET_PROPERTIES(cpuinfo_mock PROPERTIES PUBLIC_HEADER include/cpuinfo.h)
  TARGET_INCLUDE_DIRECTORIES(cpuinfo_mock PRIVATE include)
  TARGET_INCLUDE_DIRECTORIES(cpuinfo_mock PRIVATE src)
  TARGET_COMPILE_DEFINITIONS(cpuinfo_mock PRIVATE "-DCPUINFO_MOCK=1")
ENDIF()

# ---[ Helper and debug tools
IF(BUILD_TOOLS)
  ADD_EXECUTABLE(isa-info tools/isa-info.c)
  TARGET_LINK_LIBRARIES(isa-info PRIVATE cpuinfo)

  ADD_EXECUTABLE(cpu-info tools/cpu-info.c)
  TARGET_LINK_LIBRARIES(cpu-info PRIVATE cpuinfo)

  ADD_EXECUTABLE(cache-info tools/cache-info.c)
  TARGET_LINK_LIBRARIES(cache-info PRIVATE cpuinfo)

  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i686|x86_64)$")
    ADD_EXECUTABLE(cpuid-dump tools/cpuid-dump.c)
    TARGET_INCLUDE_DIRECTORIES(cpuid-dump PRIVATE src)
    TARGET_INCLUDE_DIRECTORIES(cpuid-dump PRIVATE include)
  ENDIF()
ENDIF()
