LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := helloneon
LOCAL_SRC_FILES := BoxFilterBorderNeonThread.cpp BoxFilterBorderThread.cpp BoxFilterBorder.cpp BoxFilterNeon.cpp Utils.cpp main.cpp

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), arm64-v8a armeabi-v7a x86))
    LOCAL_CFLAGS := -DHAVE_NEON=1
	LOCAL_CFLAGS := -D__AARCH_FEATURE_ADVSIMD=1
    LOCAL_CFLAGS := -D__ARM_NEON__=1
    LOCAL_CFLAGS := -D__ARM_NEON=1
    LOCAL_CFLAGS := -DWITH_NEON=1

ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -mssse3
endif
    # LOCAL_SRC_FILES += helloneon-intrinsics.c.neon
endif

LOCAL_STATIC_LIBRARIES := cpufeatures
LOCAL_LDLIBS := -llog

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS := -pie -fPIE -lz -llog -pthread

LOCAL_CFLAGS := -g -Wno-write-strings
LOCAL_CFLAGS += -mfloat-abi=softfp -mfpu=neon

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)

$(call import-module,android/cpufeatures)
