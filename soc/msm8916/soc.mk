#
# Copyright 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# TODO(leecam): Make this automatic.
LOCAL_PATH := hardware/bsp/qcom/soc/msm8916

# Arm32 device.
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a
TARGET_CPU_VARIANT := generic
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_KERNEL_ARCH := $(TARGET_ARCH)

TARGET_NO_BOOTLOADER := false
TARGET_NO_KERNEL := false

BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=msm8916 msm_rtb.filter=0x237 ehci-hcd.park=3 androidboot.bootdevice=7824900.sdhci lpm_levels.sleep_disabled=1 earlyprintk androidboot.selinux=enforcing

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_FLASH_BLOCK_SIZE := 131072

PRODUCT_COPY_FILES += \
  $(LOCAL_PATH)/init.msm8916.rc:root/init.msm8916.rc \
  system/core/rootdir/init.usb.rc:root/init.usb.rc \
  system/core/rootdir/ueventd.rc:root/ueventd.rc \

# TODO: Move common/prebuilts/sepolicy include to audio_prebuilts.mk
BOARD_SEPOLICY_DIRS += \
	$(LOCAL_PATH)/sepolicy \
	$(LOCAL_PATH)/prebuilts/sepolicy \

# Set up the local kernel.
TARGET_KERNEL_SRC := hardware/bsp/kernel/qcom/qcom-msm-3.10
TARGET_KERNEL_DEFCONFIG := msm8916-perf_defconfig
TARGET_KERNEL_CONFIGS := $(realpath $(LOCAL_PATH)/soc.kconf)

# Include prebuilts to detect audio devices.
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/prebuilts/init.audio.rc:system/etc/init/init.audio.rc \

# Audio daemons
PRODUCT_COPY_FILES += \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/daemons/rmt_storage:/system/bin/rmt_storage \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/daemons/qmuxd:/system/bin/qmuxd \

# Audio libs
PRODUCT_COPY_FILES += \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libqmi_common_so.so:/system/lib/libqmi_common_so.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libqmi_csi.so:/system/lib/libqmi_csi.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libsmemlog.so:/system/lib/libsmemlog.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libqmi_encdec.so:/system/lib/libqmi_encdec.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libconfigdb.so:/system/lib/libconfigdb.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libdiag.so:/system/lib/libdiag.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libdsutils.so:/system/lib/libdsutils.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libqmi.so:/system/lib/libqmi.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libxml.so:/system/lib/libxml.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libidl.so:/system/lib/libidl.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libmdmdetect.so:/system/lib/libmdmdetect.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libqmi_client_qmux.so:/system/lib/libqmi_client_qmux.so \
	$(TOP)/hardware/bsp/qcom/soc/msm8916/prebuilts/libs/libqmiservices.so:/system/lib/libqmiservices.so \
