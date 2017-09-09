LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)
LOCAL_MODULE := cpuinfo
LOCAL_SRC_FILES := $(LOCAL_PATH)/src/init.c \
    $(LOCAL_PATH)/src/cache.c \
    $(LOCAL_PATH)/src/log.c \
    $(LOCAL_PATH)/src/linux/processors.c \
    $(LOCAL_PATH)/src/linux/smallfile.c \
    $(LOCAL_PATH)/src/linux/multiline.c \
    $(LOCAL_PATH)/src/linux/cpulist.c
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))
LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/src/arm/uarch.c \
	$(LOCAL_PATH)/src/arm/cache.c \
	$(LOCAL_PATH)/src/arm/linux/init.c \
	$(LOCAL_PATH)/src/arm/linux/cpuinfo.c \
	$(LOCAL_PATH)/src/arm/android/properties.c \
	$(LOCAL_PATH)/src/arm/android/chipset.c
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
    $(LOCAL_PATH)/src/linux/multiline.c \
    $(LOCAL_PATH)/src/linux/cpulist.c
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))
LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/src/arm/uarch.c \
	$(LOCAL_PATH)/src/arm/cache.c \
	$(LOCAL_PATH)/src/arm/linux/init.c \
	$(LOCAL_PATH)/src/arm/linux/cpuinfo.c \
	$(LOCAL_PATH)/src/arm/android/properties.c \
	$(LOCAL_PATH)/src/arm/android/chipset.c
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
LOCAL_MODULE := gtest_main
LOCAL_SRC_FILES := $(LOCAL_PATH)/deps/googletest/googletest/src/gtest_main.cc
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/deps/googletest/googletest/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) $(LOCAL_PATH)/deps/googletest/googletest
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := init-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/init.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo gtest
include $(BUILD_EXECUTABLE)


ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a))

include $(CLEAR_VARS)
LOCAL_MODULE := blu-r1-hd-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/blu-r1-hd.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-e-gen1-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/moto-e-gen1.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

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
LOCAL_MODULE := xperia-c4-dual-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/xperia-c4-dual.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus-s-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/nexus-s.cc
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
LOCAL_MODULE := nexus5x-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/nexus5x.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus6-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/nexus6.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus6p-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/nexus6p.cc
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
LOCAL_MODULE := pixel-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/pixel.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pixel-xl-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/pixel-xl.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pixel-c-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/pixel-c.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xiaomi-redmi-2a-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/xiaomi-redmi-2a.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := meizu-pro-6s-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/meizu-pro-6s.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-grand-prime-value-edition-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-grand-prime-value-edition.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j7-tmobile-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-j7-tmobile.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j7-uae-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-j7-uae.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-c9-pro-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-c9-pro.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s3-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s3-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s4-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s4-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s5-global-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s5-global.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s5-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s5-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s6-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s6.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s7-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s7-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s7-global-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s7-global.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s8-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s8-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s8-global-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/galaxy-s8-global.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

endif # armeabi, armeabi-v7a

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))

include $(CLEAR_VARS)
LOCAL_MODULE := chipset-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/name/android-properties-interface.c \
	$(LOCAL_PATH)/test/name/proc-cpuinfo-hardware.cc \
	$(LOCAL_PATH)/test/name/ro-product-board.cc \
	$(LOCAL_PATH)/test/name/ro-board-platform.cc \
	$(LOCAL_PATH)/test/name/ro-mediatek-platform.cc \
	$(LOCAL_PATH)/test/name/ro-chipname.cc \
	$(LOCAL_PATH)/test/name/android-properties.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_STATIC_LIBRARIES := cpuinfo gtest gtest_main
include $(BUILD_EXECUTABLE)

endif # armeabi, armeabi-v7a, or arm64-v8a
