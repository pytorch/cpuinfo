LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)
LOCAL_MODULE := cpuinfo
LOCAL_SRC_FILES := $(LOCAL_PATH)/src/init.c \
    $(LOCAL_PATH)/src/cache.c \
    $(LOCAL_PATH)/src/log.c \
    $(LOCAL_PATH)/src/linux/processors.c \
    $(LOCAL_PATH)/src/linux/smallfile.c \
    $(LOCAL_PATH)/src/linux/cpulist.c
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))
LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/src/arm/uarch.c \
	$(LOCAL_PATH)/src/arm/cache.c \
	$(LOCAL_PATH)/src/arm/linux/init.c \
	$(LOCAL_PATH)/src/arm/linux/cpuinfo.c
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/arm32-isa.c.arm
endif # armeabi
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/arm32-isa.c
endif # armeabi-v7a
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/arm64-isa.c
endif # arm64-v8a
endif # armeabi, armeabi-v7a, or arm64-v8a
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),x86 x86_64))
LOCAL_SRC_FILES += \
    $(LOCAL_PATH)/src/x86/init.c \
    $(LOCAL_PATH)/src/x86/info.c \
    $(LOCAL_PATH)/src/x86/isa.c \
    $(LOCAL_PATH)/src/x86/vendor.c \
    $(LOCAL_PATH)/src/x86/uarch.c \
    $(LOCAL_PATH)/src/x86/topology.c \
    $(LOCAL_PATH)/src/x86/cache/init.c \
    $(LOCAL_PATH)/src/x86/cache/descriptor.c \
    $(LOCAL_PATH)/src/x86/cache/deterministic.c \
    $(LOCAL_PATH)/src/x86/linux/init.c
endif # x86 or x86_64
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu99 -D_GNU_SOURCE=1
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cpuinfo_mock
LOCAL_SRC_FILES := $(LOCAL_PATH)/src/init.c \
    $(LOCAL_PATH)/src/cache.c \
    $(LOCAL_PATH)/src/log.c \
    $(LOCAL_PATH)/src/linux/mockfile.c \
    $(LOCAL_PATH)/src/linux/processors.c \
    $(LOCAL_PATH)/src/linux/smallfile.c \
    $(LOCAL_PATH)/src/linux/cpulist.c
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))
LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/src/arm/uarch.c \
	$(LOCAL_PATH)/src/arm/cache.c \
	$(LOCAL_PATH)/src/arm/linux/init.c \
	$(LOCAL_PATH)/src/arm/linux/cpuinfo.c
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/arm32-isa.c.arm
endif # armeabi
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/arm32-isa.c
endif # armeabi-v7a
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/arm64-isa.c
endif # arm64-v8a
endif # armeabi, armeabi-v7a, or arm64-v8a
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),x86 x86_64))
LOCAL_SRC_FILES += \
    $(LOCAL_PATH)/src/x86/init.c \
    $(LOCAL_PATH)/src/x86/info.c \
    $(LOCAL_PATH)/src/x86/isa.c \
    $(LOCAL_PATH)/src/x86/vendor.c \
    $(LOCAL_PATH)/src/x86/uarch.c \
    $(LOCAL_PATH)/src/x86/topology.c \
    $(LOCAL_PATH)/src/x86/cache/init.c \
    $(LOCAL_PATH)/src/x86/cache/descriptor.c \
    $(LOCAL_PATH)/src/x86/cache/deterministic.c \
    $(LOCAL_PATH)/src/x86/linux/init.c
endif # x86 or x86_64
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu99 -D_GNU_SOURCE=1 -DCPUINFO_LOG_LEVEL=4 -DCPUINFO_MOCK=1
LOCAL_EXPORT_CFLAGS := -DCPUINFO_MOCK=1
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cpu-info
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/cpu-info.c
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := isa-info
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/isa-info.c
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := cache-info
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/cache-info.c
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := gtest
LOCAL_SRC_FILES := $(LOCAL_PATH)/deps/googletest/googletest/src/gtest-all.cc
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/deps/googletest/googletest/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) $(LOCAL_PATH)/deps/googletest/googletest
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_EXPORT_LDLIBS := -latomic
endif # armeabi
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := init-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/init.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo gtest
include $(BUILD_EXECUTABLE)


ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a))

include $(CLEAR_VARS)
LOCAL_MODULE := moto-g-gen2-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/moto-g-gen2.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-g-gen3-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/moto-g-gen3.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus4-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/nexus4.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus9-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/nexus9.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s7-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s7-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

endif # armeabi, armeabi-v7a
