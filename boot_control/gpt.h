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

#ifndef HARDWARE_QCOM_BOOTCTL_HAL_GPT_H
#define HARDWARE_QCOM_BOOTCTL_HAL_GPT_H

#include <string>
#include <assert.h>

#define GPT_HDR_SIGNATURE               0x5452415020494645ULL
#define MAX_GPT_ENTRIES                 128
#define GPT_NAME_NUM_CHARS              36
#define MAX_SLOTS                       2

#define PART_ATT_SUCCESS_OFFSET         56
#define PART_ATT_TRIES_OFFSET           52
#define PART_ATT_PRIORITY_OFFSET        48

#define PART_ATT_SUCCESS_MASK           (((uint64_t)0x1) << PART_ATT_SUCCESS_OFFSET)
#define PART_ATT_TRIES_MASK             (((uint64_t)0xF) << PART_ATT_TRIES_OFFSET)
#define PART_ATT_PRIORITY_MASK          (((uint64_t)0xF) << PART_ATT_PRIORITY_OFFSET)
#define PART_ATT_ALL_MASK               (PART_ATT_SUCCESS_MASK | \
                                         PART_ATT_PRIORITY_MASK | \
                                         PART_ATT_TRIES_MASK)

namespace qcom_boot_control {

typedef struct
{
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t  data4[8];
} __attribute__((packed)) guid_t;

typedef struct gpt_header {
    uint64_t signature;
    uint32_t rev;
    uint32_t hdr_size;
    uint32_t hdr_crc32;
    uint32_t reserved;
    uint64_t curr_lba;
    uint64_t backup_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    guid_t   disk_guid;
    uint64_t start_lba_part_array;
    uint32_t num_parts;
    uint32_t part_size;
    uint32_t part_arr_crc32;
    uint8_t  reserved_zeros[420];
} __attribute__((packed)) gpt_header_t;

typedef struct gpt_partition_entry {
    guid_t   type_guid;
    guid_t   unique_partition_guid;
    uint64_t first_lba;
    uint64_t last_lba;
    uint64_t attribute_flag;
    char16_t  name[GPT_NAME_NUM_CHARS];
} __attribute__((packed)) gpt_partition_entry_t;

class PartitionTables {
public:
   /*
    * read_partitions() reads the header and partition array from
    * primary or secondary GPT of the disk. Secondary GPT is read in case
    * primary GPT fails sanity check, errni is set if both GPT copies
    * fail sanity check.
    * Returns pointer to allocated object on success,
    * and 'nullptr' on error with errno set.
    * On failure this method will log to stderr.
    */
    static std::unique_ptr<PartitionTables>
                           read_partitions(const std::string& device);

   /*
    * write_partitions() writes the header and partition array to
    * primary and secondary GPT on the disk.
    * Returns 0 on success, -errno on error.
    */
    int write_partitions();

    /*
     * getIndexForSlottedBootPartition() gets the partition index associated
     * with the slot parameter passed.
     * Returns 0 on success, and partition_index value is valid.
     * REtrns -errno on error, the partition_index value is invalid.
     */
    int getIndexForSlottedBootPartition(unsigned slot,
                                        uint32_t& partition_index) const;

    /*
     * gpt_sanity_check() checks the header for correctness of signature,
     * size and CRC.
     * Returns 'true' on success, 'false' on error.
     */
    bool gpt_sanity_check();

    gpt_header_t header;
    gpt_partition_entry_t partition_array[MAX_GPT_ENTRIES];

private:
    std::string disk_device = "";
};

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
static_assert(sizeof(guid_t) == 16, "struct guid_t has wrong size");
static_assert(sizeof(gpt_header_t) == 512,
               "struct gpt_header_t has wrong size");
static_assert(sizeof(gpt_partition_entry_t) == 128,
               "struct gpt_partition_entry_t has wrong size");
#endif

}; //qcom_boot_control

#endif
