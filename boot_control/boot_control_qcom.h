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

#ifndef HARDWARE_QCOM_BOOTCTL_HAL_H
#define HARDWARE_QCOM_BOOTCTL_HAL_H

#include <hardware/hardware.h>
#include <hardware/boot_control.h>

namespace qcom_boot_control {

#define BLK_DEV_NODE                    "/dev/block/mmcblk0"

typedef enum {
    SLOT_PROPERTY_ATTRIBUTE_FLAGS,
    SLOT_PROPERTY_PARTITION_INDEX,
} slot_prop_t;

void init(boot_control_module_t *module);
unsigned getNumberSlots(boot_control_module_t *module);
unsigned getCurrentSlot(boot_control_module_t *module);
int markBootSuccessful(boot_control_module_t *module);
int setActiveBootSlot(boot_control_module_t *module, unsigned slot);
int setSlotAsUnbootable(struct boot_control_module *module, unsigned slot);
int isSlotBootable(struct boot_control_module *module, unsigned slot);
const char* getSuffix(boot_control_module_t *module, unsigned slot);

}; //namespace qcom_boot_control
#endif
