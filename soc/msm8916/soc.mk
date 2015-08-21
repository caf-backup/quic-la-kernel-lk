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

BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=brillo msm_rtb.filter=0x237 ehci-hcd.park=3 androidboot.bootdevice=7824900.sdhci lpm_levels.sleep_disabled=1 earlyprintk androidboot.selinux=enforcing

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_FLASH_BLOCK_SIZE := 131072

PRODUCT_COPY_FILES += \
  $(LOCAL_PATH)/init.soc.rc:root/initrc.d/init.soc.rc \
  system/core/rootdir/init.usb.rc:root/init.usb.rc \
  system/core/rootdir/ueventd.rc:root/ueventd.rc \

# TODO: Move common/prebuilts/sepolicy include to audio_prebuilts.mk
BOARD_SEPOLICY_DIRS += \
	$(LOCAL_PATH)/sepolicy \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/sepolicy

# Set up the local kernel.
TARGET_KERNEL_SRC := hardware/bsp/kernel/qcom/android-msm-3.10
TARGET_KERNEL_DEFCONFIG := dragonboard_defconfig
