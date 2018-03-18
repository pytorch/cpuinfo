LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)
LOCAL_MODULE := cpuinfo
LOCAL_SRC_FILES := $(LOCAL_PATH)/src/init.c \
	$(LOCAL_PATH)/src/api.c \
	$(LOCAL_PATH)/src/log.c \
	$(LOCAL_PATH)/src/gpu/gles2.c \
	$(LOCAL_PATH)/src/linux/gpu.c \
	$(LOCAL_PATH)/src/linux/current.c \
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
	$(LOCAL_PATH)/src/arm/linux/clusters.c \
	$(LOCAL_PATH)/src/arm/linux/chipset.c \
	$(LOCAL_PATH)/src/arm/linux/midr.c \
	$(LOCAL_PATH)/src/arm/linux/hwcap.c \
	$(LOCAL_PATH)/src/arm/android/gpu.c \
	$(LOCAL_PATH)/src/arm/android/properties.c
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/aarch32-isa.c.arm
endif # armeabi
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/aarch32-isa.c
endif # armeabi-v7a
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/aarch64-isa.c
endif # arm64-v8a
endif # armeabi, armeabi-v7a, or arm64-v8a
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),x86 x86_64))
LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/src/x86/init.c \
	$(LOCAL_PATH)/src/x86/info.c \
	$(LOCAL_PATH)/src/x86/name.c \
	$(LOCAL_PATH)/src/x86/isa.c \
	$(LOCAL_PATH)/src/x86/vendor.c \
	$(LOCAL_PATH)/src/x86/uarch.c \
	$(LOCAL_PATH)/src/x86/topology.c \
	$(LOCAL_PATH)/src/x86/cache/init.c \
	$(LOCAL_PATH)/src/x86/cache/descriptor.c \
	$(LOCAL_PATH)/src/x86/cache/deterministic.c \
	$(LOCAL_PATH)/src/x86/linux/cpuinfo.c \
	$(LOCAL_PATH)/src/x86/linux/init.c
endif # x86 or x86_64
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu99 -Wall -Wno-maybe-uninitialized -Wno-unknown-warning-option -D_GNU_SOURCE=1
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cpuinfo_mock
LOCAL_SRC_FILES := $(LOCAL_PATH)/src/init.c \
	$(LOCAL_PATH)/src/api.c \
	$(LOCAL_PATH)/src/log.c \
	$(LOCAL_PATH)/src/gpu/gles2-mock.c \
	$(LOCAL_PATH)/src/linux/gpu.c \
	$(LOCAL_PATH)/src/linux/current.c \
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
	$(LOCAL_PATH)/src/arm/linux/clusters.c \
	$(LOCAL_PATH)/src/arm/linux/chipset.c \
	$(LOCAL_PATH)/src/arm/linux/midr.c \
	$(LOCAL_PATH)/src/arm/linux/hwcap.c \
	$(LOCAL_PATH)/src/arm/android/gpu.c \
	$(LOCAL_PATH)/src/arm/android/properties.c
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/aarch32-isa.c.arm
endif # armeabi
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/aarch32-isa.c
endif # armeabi-v7a
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES += $(LOCAL_PATH)/src/arm/linux/aarch64-isa.c
endif # arm64-v8a
endif # armeabi, armeabi-v7a, or arm64-v8a
ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),x86 x86_64))
LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/src/x86/init.c \
	$(LOCAL_PATH)/src/x86/info.c \
	$(LOCAL_PATH)/src/x86/name.c \
	$(LOCAL_PATH)/src/x86/isa.c \
	$(LOCAL_PATH)/src/x86/vendor.c \
	$(LOCAL_PATH)/src/x86/uarch.c \
	$(LOCAL_PATH)/src/x86/topology.c \
	$(LOCAL_PATH)/src/x86/mockcpuid.c \
	$(LOCAL_PATH)/src/x86/cache/init.c \
	$(LOCAL_PATH)/src/x86/cache/descriptor.c \
	$(LOCAL_PATH)/src/x86/cache/deterministic.c \
	$(LOCAL_PATH)/src/x86/linux/cpuinfo.c \
	$(LOCAL_PATH)/src/x86/linux/init.c
endif # x86 or x86_64
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu99 -Wall -Wno-maybe-uninitialized -Wno-unknown-warning-option -D_GNU_SOURCE=1 -DCPUINFO_LOG_LEVEL=4 -DCPUINFO_MOCK=1
LOCAL_EXPORT_CFLAGS := -DCPUINFO_MOCK=1
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cpu-info
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/cpu-info.c
LOCAL_CFLAGS := -std=gnu99
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := isa-info
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/isa-info.c
LOCAL_CFLAGS := -std=gnu99
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := cache-info
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/cache-info.c
LOCAL_CFLAGS := -std=gnu99
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

# include $(CLEAR_VARS)
# LOCAL_MODULE := gpu-dump
# LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/gpu-dump.c
# LOCAL_CFLAGS := -std=gnu99
# LOCAL_LDLIBS := -ldl -lEGL -lGLESv2
# include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := size-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/size.c
LOCAL_CFLAGS := -std=gnu99
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_EXECUTABLE)

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))

include $(CLEAR_VARS)
LOCAL_MODULE := auxv-dump
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/auxv-dump.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu99
include $(BUILD_EXECUTABLE)

endif # armeabi, armeabi-v7a, or arm64-v8a

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),x86 x86_64))

include $(CLEAR_VARS)
LOCAL_MODULE := cpuid-dump
LOCAL_SRC_FILES := $(LOCAL_PATH)/tools/cpuid-dump.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu99
include $(BUILD_EXECUTABLE)

endif # x86, or x86_64

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

include $(CLEAR_VARS)
LOCAL_MODULE := get-current-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/get-current.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test
LOCAL_STATIC_LIBRARIES := cpuinfo gtest
include $(BUILD_EXECUTABLE)

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a))

include $(CLEAR_VARS)
LOCAL_MODULE := atm7029b-tablet-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/atm7029b-tablet.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := blu-r1-hd-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/blu-r1-hd.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-a3-2016-eu-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-a3-2016-eu.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-a8-2016-duos-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-a8-2016-duos.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-grand-prime-value-edition-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-grand-prime-value-edition.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j1-2016-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-j1-2016.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j5-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-j5.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j7-prime-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-j7-prime.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j7-tmobile-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-j7-tmobile.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-j7-uae-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-j7-uae.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s3-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s3-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s4-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s4-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s5-global-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s5-global.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s5-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s5-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-tab-3-7.0-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-tab-3-7.0.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-tab-3-lite-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-tab-3-lite.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-win-duos-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-win-duos.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := huawei-ascend-p7-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/huawei-ascend-p7.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := lenovo-a6600-plus-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/lenovo-a6600-plus.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := lenovo-vibe-x2-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/lenovo-vibe-x2.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := lg-k10-eu-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/lg-k10-eu.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := lg-optimus-g-pro-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/lg-optimus-g-pro.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-e-gen1-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/moto-e-gen1.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-g-gen1-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/moto-g-gen1.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-g-gen2-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/moto-g-gen2.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-g-gen3-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/moto-g-gen3.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := moto-g-gen4-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/moto-g-gen4.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus-s-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus-s.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus4-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus4.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus6-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus6.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus10-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus10.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := padcod-10.1-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/padcod-10.1.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xiaomi-redmi-2a-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/xiaomi-redmi-2a.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xperia-sl-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/xperia-sl.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

endif # armeabi, armeabi-v7a

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),armeabi armeabi-v7a arm64-v8a))

include $(CLEAR_VARS)
LOCAL_MODULE := android_properties_interface
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/name/android-properties-interface.c
LOCAL_CFLAGS := -std=gnu99
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_STATIC_LIBRARIES := cpuinfo
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := chipset-test
LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/test/name/proc-cpuinfo-hardware.cc \
	$(LOCAL_PATH)/test/name/ro-product-board.cc \
	$(LOCAL_PATH)/test/name/ro-board-platform.cc \
	$(LOCAL_PATH)/test/name/ro-mediatek-platform.cc \
	$(LOCAL_PATH)/test/name/ro-arch.cc \
	$(LOCAL_PATH)/test/name/ro-chipname.cc \
	$(LOCAL_PATH)/test/name/android-properties.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_STATIC_LIBRARIES := android_properties_interface gtest gtest_main
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := cache-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/arm-cache.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_CFLAGS := -std=gnu++11 -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
LOCAL_STATIC_LIBRARIES := cpuinfo gtest gtest_main
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := alcatel-revvl-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/alcatel-revvl.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-a8-2018-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-a8-2018.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-c9-pro-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-c9-pro.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s6-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s6.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s7-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s7-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s7-global-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s7-global.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s8-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s8-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s8-global-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s8-global.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := galaxy-s9-us-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/galaxy-s9-us.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := huawei-mate-8-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/huawei-mate-8.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := huawei-mate-9-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/huawei-mate-9.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := huawei-mate-10-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/huawei-mate-10.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := huawei-p8-lite-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/huawei-p8-lite.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := huawei-p9-lite-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/huawei-p9-lite.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := iconia-one-10-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/iconia-one-10.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := meizu-pro-6-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/meizu-pro-6.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := meizu-pro-6s-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/meizu-pro-6s.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := meizu-pro-7-plus-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/meizu-pro-7-plus.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus5x-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus5x.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus6p-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus6p.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := nexus9-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/nexus9.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := oneplus-3t-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/oneplus-3t.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := oneplus-5-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/oneplus-5.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := oneplus-5t-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/oneplus-5t.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := oppo-a37-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/oppo-a37.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := oppo-r9-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/oppo-r9.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pixel-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/pixel.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pixel-xl-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/pixel-xl.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pixel-c-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/pixel-c.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pixel-2-xl-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/pixel-2-xl.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xiaomi-mi-5c-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/xiaomi-mi-5c.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xiaomi-redmi-note-3-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/xiaomi-redmi-note-3.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xiaomi-redmi-note-4-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/xiaomi-redmi-note-4.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := xperia-c4-dual-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/xperia-c4-dual.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

endif # armeabi, armeabi-v7a, or arm64-v8a

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI),x86 x86_64))

include $(CLEAR_VARS)
LOCAL_MODULE := brand-string-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/name/brand-string.cc
LOCAL_STATIC_LIBRARIES := cpuinfo gtest gtest_main
include $(BUILD_EXECUTABLE)

endif # x86, or x86_64

ifeq ($(TARGET_ARCH_ABI),x86)

include $(CLEAR_VARS)
LOCAL_MODULE := alldocube-iwork8-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/alldocube-iwork8.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := memo-pad-7-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/memo-pad-7.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := zenfone-c-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/zenfone-c.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := zenfone-2-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/zenfone-2.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := zenfone-2e-test
LOCAL_SRC_FILES := $(LOCAL_PATH)/test/mock/zenfone-2e.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/test/mock
LOCAL_STATIC_LIBRARIES := cpuinfo_mock gtest
include $(BUILD_EXECUTABLE)

endif # x86
