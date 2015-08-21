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

PRODUCT_PACKAGES += \
	init.audio.rc

# Audio daemons
PRODUCT_COPY_FILES += \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/daemons/rmt_storage:/system/bin/rmt_storage \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/daemons/qmuxd:/system/bin/qmuxd \

# Audio libs
PRODUCT_COPY_FILES += \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libqmi_common_so.so:/system/lib/libqmi_common_so.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libqmi_csi.so:/system/lib/libqmi_csi.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libsmemlog.so:/system/lib/libsmemlog.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libqmi_encdec.so:/system/lib/libqmi_encdec.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libconfigdb.so:/system/lib/libconfigdb.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libdiag.so:/system/lib/libdiag.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libdsutils.so:/system/lib/libdsutils.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libqmi.so:/system/lib/libqmi.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libxml.so:/system/lib/libxml.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libidl.so:/system/lib/libidl.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libmdmdetect.so:/system/lib/libmdmdetect.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libqmi_client_qmux.so:/system/lib/libqmi_client_qmux.so \
	$(TOP)/hardware/bsp/qcom/common/prebuilts/libs/libqmiservices.so:/system/lib/libqmiservices.so \
