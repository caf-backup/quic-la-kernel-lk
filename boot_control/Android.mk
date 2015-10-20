# Copyright 2015 The Android Open Source Project

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
		gpt.cpp \
		boot_control_qcom.cpp

LOCAL_CFLAGS := -Wall -Wno-missing-field-initializers
LOCAL_C_INCLUDES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include

LOCAL_SHARED_LIBRARIES := libcutils libutils

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE:= bootctrl.msm8916
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
