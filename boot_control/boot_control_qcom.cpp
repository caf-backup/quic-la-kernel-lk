/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "boot_control_hw"
#define LOG_NDEBUG 0

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <cutils/log.h>
#include <hardware/hardware.h>
#include <cutils/properties.h>
#include <hardware/boot_control.h>

#include "boot_control_qcom.h"
#include "gpt.h"

using namespace std;

/* Qualcomm boot_control HAL implements reading A/B slot information
 * from the contents of GPT.
 */
static struct hw_module_methods_t module_methods = {
    .open  = nullptr,
};

boot_control_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag                 = HARDWARE_MODULE_TAG,
        .module_api_version  = BOOT_CONTROL_MODULE_API_VERSION_0_1,
        .hal_api_version     = HARDWARE_HAL_API_VERSION,
        .id                  = BOOT_CONTROL_HARDWARE_MODULE_ID,
        .name                = "Qualcomm boot_control HAL",
        .author              = "CodeAurora Forum",
        .methods             = &module_methods,
    },
    .init                 = qcom_boot_control::init,
    .getNumberSlots       = qcom_boot_control::getNumberSlots,
    .getCurrentSlot       = qcom_boot_control::getCurrentSlot,
    .markBootSuccessful   = qcom_boot_control::markBootSuccessful,
    .setActiveBootSlot    = qcom_boot_control::setActiveBootSlot,
    .setSlotAsUnbootable  = qcom_boot_control::setSlotAsUnbootable,
    .isSlotBootable       = qcom_boot_control::isSlotBootable,
    .getSuffix            = qcom_boot_control::getSuffix,
};

namespace qcom_boot_control {

void init(boot_control_module_t *module)
{
    ALOGV("QCOM boot control HAL.");
}

unsigned getNumberSlots(boot_control_module_t *module)
{
    return MAX_SLOTS;
}

unsigned getCurrentSlot(boot_control_module_t *module)
{
    char propbuf[PROPERTY_VALUE_MAX];

    property_get("ro.boot.slot_suffix", propbuf, "");
    ALOGV("getCurrentSlot: slot suffix %s", propbuf);

    if (!strcmp(propbuf, "_a")) {
        return 0;
    } else if (!strcmp(propbuf, "_b")) {
        return 1;
    } else {
        ALOGE("ERROR: unsupported slot suffix");
        return 0;
    }
    return 0;
}

int markBootSuccessful(boot_control_module_t *module)
{
    std::unique_ptr<PartitionTables> gpt =
                PartitionTables::read_partitions(BLK_DEV_NODE);
    uint32_t partition_index;
    unsigned slot;
    int ret;

    if (gpt == nullptr) {
        ALOGE("markBootSuccessful: read partition returns %d", errno);
        return -errno;
    }

    slot = getCurrentSlot(module);

    if (slot >= MAX_SLOTS)
        return -EINVAL;

    if ((ret = gpt->getIndexForSlottedBootPartition(slot, partition_index)))
        return ret;

    /*Clear set success and clear tries count.*/
    gpt->partition_array[partition_index].attribute_flag |=
                                            PART_ATT_SUCCESS_MASK;
    gpt->partition_array[partition_index].attribute_flag &=
                                            ~PART_ATT_TRIES_MASK;

    if ((ret = gpt->write_partitions())) {
        ALOGE("markBootSuccessful: write partition returns %d", ret);
        return ret;
    }

    ALOGV("markBootSuccessful: slot:%d partition:%ld", slot, partition_index);

    return 0;
}

int setActiveBootSlot(boot_control_module_t *module, unsigned slot)
{
    std::unique_ptr<PartitionTables> gpt =
                PartitionTables::read_partitions(BLK_DEV_NODE);
    uint32_t partition_index;
    unsigned other_slot = 1 - slot;
    uint64_t attribute_flags;
    int ret;

    if (gpt == nullptr) {
        ALOGE("setActiveBootSlot: read partition returns %d", errno);
        return -errno;
    }

    if (slot >= MAX_SLOTS)
        return -EINVAL;

    if ((ret = gpt->getIndexForSlottedBootPartition( slot, partition_index)))
        return ret;

    /*Set priority = 15 and try count = 7 for the target slot */
    attribute_flags =
                ((15ULL << PART_ATT_PRIORITY_OFFSET) & PART_ATT_PRIORITY_MASK) |
                ((7ULL << PART_ATT_TRIES_OFFSET) & PART_ATT_TRIES_MASK);

    gpt->partition_array[partition_index].attribute_flag &= ~PART_ATT_ALL_MASK;
    gpt->partition_array[partition_index].attribute_flag |= attribute_flags;

    if ((ret = gpt->getIndexForSlottedBootPartition(other_slot, partition_index)))
        return ret;

    /*Modify priority for other slot if it has a non-zero priority */
    if (gpt->partition_array[partition_index].attribute_flag &
                                                    PART_ATT_PRIORITY_MASK) {
         gpt->partition_array[partition_index].attribute_flag &=
                                                    ~PART_ATT_PRIORITY_MASK;
         gpt->partition_array[partition_index].attribute_flag |=
                (14ULL << PART_ATT_PRIORITY_OFFSET) & PART_ATT_PRIORITY_MASK;
    }

    if ((ret = gpt->write_partitions())) {
        ALOGE("setActiveBootSlot: write partition returns %d", ret);
        return ret;
    }

    ALOGV("setActiveBootSlot: slot %d", slot);
    return 0;
}

int setSlotAsUnbootable(struct boot_control_module *module, unsigned slot)
{
    std::unique_ptr<PartitionTables> gpt =
                PartitionTables::read_partitions(BLK_DEV_NODE);
    uint32_t partition_index;
    int ret = 0;

    if (gpt == nullptr) {
        ALOGV("setSlotAsUnbootable: read partition returns %d", -errno);
        return -errno;
    }

    if (slot >= MAX_SLOTS)
        return -EINVAL;

    if ((ret = gpt->getIndexForSlottedBootPartition(slot, partition_index)))
        return ret;

    gpt->partition_array[partition_index].attribute_flag &= ~PART_ATT_ALL_MASK;

    if ((ret = gpt->write_partitions())) {
        ALOGE("setSlotAsUnbootable: write partition returns %d", ret);
        return ret;
    }

    ALOGV("setSlotAsUnbootable: partition index: %d, ret: %d",
          partition_index, ret);

    return 0;
}

int isSlotBootable(struct boot_control_module *module, unsigned slot)
{
    std::unique_ptr<PartitionTables> gpt =
                PartitionTables::read_partitions(BLK_DEV_NODE);
    uint32_t partition_index = 0;
    int ret = 0;

    if (gpt == nullptr) {
        ALOGE("isSlotBootable: read partition returns %d", -errno);
        return -errno;
    }

    if (slot >= MAX_SLOTS)
        return -EINVAL;

    if ((ret = gpt->getIndexForSlottedBootPartition(slot, partition_index)))
        return ret;

    if (gpt->partition_array[partition_index].attribute_flag &
                PART_ATT_SUCCESS_MASK)
        ret = 1;

    ALOGV("isSlotBootable: Slot: %d attribute: %llx, ret: %d",
          slot, gpt->partition_array[partition_index].attribute_flag, ret);

    return ret;
}

const char* getSuffix(boot_control_module_t *module, unsigned slot)
{
    static const char* suffix[2] = {"_a", "_b"};
    if (slot >= MAX_SLOTS)
        return nullptr;
    return suffix[slot];
}

}; //namespace qcom_boot_control
