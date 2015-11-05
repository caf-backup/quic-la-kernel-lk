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
$(call add_kernel_configs, $(realpath $(LOCAL_PATH)/soc.kconf))

# Default Keystore HAL.
DEVICE_PACKAGES += \
	keystore.default

# Include Qualcomm Bool Control HAL.
DEVICE_PACKAGES += \
	bootctrl.msm8916

# Include Qualcomm Audio HAL implementation.
DEVICE_PACKAGES += \
	audio.primary.msm8916

# Include prebuilts to detect audio devices.
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/prebuilts/audio.rc:system/etc/init/audio.rc \

PRODUCT_LIBRARY_PATH := $(TOP)/vendor/bsp/qcom/device/dragonboard/linux_410c_board_support_package_LA.BR.1.2.4_rb1.10

# Audio daemons.
PRODUCT_COPY_FILES += \
	$(PRODUCT_LIBRARY_PATH)/bin/rmt_storage:/system/bin/rmt_storage \
	$(PRODUCT_LIBRARY_PATH)/bin/qmuxd:/system/bin/qmuxd \

# Audio libs.
PRODUCT_COPY_FILES += \
	$(PRODUCT_LIBRARY_PATH)/lib/libqmi_common_so.so:/system/lib/libqmi_common_so.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libqmi_csi.so:/system/lib/libqmi_csi.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libsmemlog.so:/system/lib/libsmemlog.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libqmi_encdec.so:/system/lib/libqmi_encdec.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libconfigdb.so:/system/lib/libconfigdb.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libdiag.so:/system/lib/libdiag.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libdsutils.so:/system/lib/libdsutils.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libqmi.so:/system/lib/libqmi.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libxml.so:/system/lib/libxml.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libidl.so:/system/lib/libidl.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libmdmdetect.so:/system/lib/libmdmdetect.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libqmi_client_qmux.so:/system/lib/libqmi_client_qmux.so \
	$(PRODUCT_LIBRARY_PATH)/lib/libqmiservices.so:/system/lib/libqmiservices.so \
