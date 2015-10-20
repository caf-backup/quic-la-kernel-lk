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

#include <string>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <cutils/log.h>
#include <utils/Unicode.h>

#include "gpt.h"

using namespace std;

namespace {

static uint64_t lba_to_offset(uint64_t lba)
{
    return lba * 512;
}

/*
 * A8h reflected is 15h, i.e. 10101000 <--> 00010101
 */
int reflect(int data, int len)
{
    int ref = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (data & 0x1) {
            ref |= (1 << ((len - 1) - i));
        }
        data = (data >> 1);
    }
    return ref;
}

uint32_t calculate_crc32(void *buf, uint32_t len)
{
    uint32_t i, j;
    uint32_t byte_length = 8;       /*length of unit (i.e. byte) */
    int msb = 0;
    int polynomial = 0x04C11DB7;    /* IEEE 32bit polynomial */
    unsigned int regs = 0xFFFFFFFF; /* init to all ones */
    int regs_mask = 0xFFFFFFFF;     /* ensure only 32 bit answer */
    int regs_msb = 0;
    unsigned int reflected_regs;

    for (i = 0; i < len; i++) {
        int data_byte = *((uint8_t *)buf + i);
        data_byte = reflect(data_byte, 8);
        for (j = 0; j < byte_length; j++) {
            msb = data_byte >> (byte_length - 1); /* get MSB */
            msb &= 1;                             /* ensure just 1 bit */
            regs_msb = (regs >> 31) & 1;          /* MSB of regs */
            regs = regs << 1;                     /* shift regs for CRC-CCITT */
            if (regs_msb ^ msb) {                 /* MSB is a 1 */
                regs = regs ^ polynomial;         /* XOR with generator poly */
            }
            regs = regs & regs_mask;              /* Mask off excess upper bits */
            data_byte <<= 1;                      /* get to next bit */
        }
    }
    regs = regs & regs_mask;
    reflected_regs = reflect(regs, 32) ^ 0xFFFFFFFF;

    return reflected_regs;
}

int open_disk(const std::string& device, bool open_for_write)
{
    int fd;
    int oflag = open_for_write ? O_RDWR : O_RDONLY;

    fd = open (device.c_str(), oflag);
    if (-1 == fd)
        return -errno;

    ALOGV("open_disk: %s", device.c_str());
    return fd;
}

ssize_t read_from_disk(const std::string& device, uint64_t offset,
                       size_t size, void *buf)
{
    ssize_t read_bytes;
    ssize_t ret = 0;
    int fd = open_disk(device, false);

    if (-1 == fd)
        return fd; // fd is set to -errno from open_disk(bool)

    if (-1 == lseek64(fd, offset, SEEK_SET)) {
        close(fd);
        return -errno;
    }

    do {
        read_bytes = read(fd, buf, size);
    } while (-1 == read_bytes && EINTR == errno);

    if (-1 == read_bytes)
        ret = -errno;
    else
        ret = read_bytes;

    close(fd);
    return ret;
}

ssize_t write_to_disk(const std::string& device, uint64_t offset,
                      size_t size, void *buf)
{
    ssize_t written_bytes;
    ssize_t ret = 0;
    int fd = open_disk (device, true);

    if (-1 == fd)
        return fd; // fd is set to -errno from open_disk(true)

    if (-1 == lseek64(fd, offset, SEEK_SET)) {
        close(fd);
        return -errno;
    }

    do {
        written_bytes = write(fd, buf, size);
    } while(-1 == written_bytes && EINTR == errno);


    if (-1 == written_bytes)
        ret = -errno;
    else
        ret = written_bytes;

    close(fd);
    return ret;
}

};

namespace qcom_boot_control {

int PartitionTables::write_partitions()
{
    ssize_t ret = 0;
    uint64_t size_num_bytes = 0;
    uint64_t num_lbas = 0;
    int fd = open(disk_device.c_str(), O_RDONLY);

    if (-1 == fd)
        return -errno;

    ret = ioctl(fd, BLKGETSIZE64, &size_num_bytes);

    close(fd);

    if (-1 == ret)
        return -errno;

    num_lbas = size_num_bytes / 512;

    ALOGV("Total disk LBAs %lld", num_lbas);

    /* Set header's fields for secondary partition */
    header.curr_lba = num_lbas - 1;
    header.backup_lba = 1;
    header.start_lba_part_array = num_lbas - 33;

    /* Calculate CRCs for secondary header */
    header.hdr_crc32 = 0;
    header.part_arr_crc32 = calculate_crc32(partition_array,
                                            header.num_parts *
                                            sizeof(gpt_partition_entry_t));
    header.hdr_crc32 = calculate_crc32(&header, header.hdr_size);

    /* Write secondary partition array to disk*/
    if ((ret = write_to_disk(disk_device, lba_to_offset(num_lbas - 33),
                             sizeof(partition_array), partition_array)) < 0)
        return ret;

    /* Write secondary header to disk*/
    if ((ret = write_to_disk(disk_device, lba_to_offset(num_lbas - 1),
                             sizeof(gpt_header_t), &header)) < 0)
        return ret;

    /* Set header's fields for primary partition */
    header.curr_lba = 1;
    header.backup_lba = num_lbas - 1;
    header.start_lba_part_array = 2;

    /* Calculate CRCs for primary header */
    header.hdr_crc32 = 0;
    header.part_arr_crc32 = calculate_crc32(partition_array,
                                            header.num_parts *
                                            sizeof(gpt_partition_entry_t));
    header.hdr_crc32 = calculate_crc32(&header, header.hdr_size);

    /* Write primary partition array to disk*/
    if ((ret = write_to_disk(disk_device, lba_to_offset(2),
                             sizeof(partition_array), partition_array)) < 0)
        return ret;

    /* Write primary header to disk*/
    if ((ret = write_to_disk(disk_device, lba_to_offset(1),
                             sizeof(gpt_header_t), &header)) < 0)
        return ret;

    return 0;
}

std::unique_ptr<PartitionTables>
               PartitionTables::read_partitions(const std::string& device)
{
    std::unique_ptr<PartitionTables> gpt(new PartitionTables);
    ssize_t ret;
    uint64_t size_num_bytes = 0;
    uint64_t num_lbas = 0;
    int fd = open(device.c_str(), O_RDONLY);

    if (-1 == fd)
        return nullptr;

    gpt->disk_device = device;

    ret = ioctl(fd, BLKGETSIZE64, &size_num_bytes);

    close(fd);

    if (-1 == ret)
        return nullptr;

    num_lbas = size_num_bytes / 512;

    ALOGV("Total disk LBAs %lld", num_lbas);

    /* Read primary header*/
    if ((ret = read_from_disk(device, lba_to_offset(1),
                              sizeof(gpt_header_t), &gpt->header)) < 0)
        return nullptr;

    if (gpt->gpt_sanity_check()) {
        /* Read primary paritition array.*/
        if ((ret = read_from_disk(device, lba_to_offset(2),
                                  sizeof(partition_array),
                                  gpt->partition_array)) < 0)
            return nullptr;

        /* Primary header and partiiton array read*/
        return gpt;
    }

    /*
     * The seconadary header and partition array is used only in case
     * the primary header fails sanity check.
     */
    ALOGE("Attempting to read secondary header and partition array.");

    /* Read secondary header. */
    if ((ret = read_from_disk(device, lba_to_offset(num_lbas - 1),
                              sizeof(gpt_header_t), &gpt->header)) < 0)
        return nullptr;

    if (gpt->gpt_sanity_check()) {
        /* Read secondary partition array.*/
        if ((ret = read_from_disk(device, lba_to_offset(num_lbas - 33),
                                  sizeof(partition_array),
                                  gpt->partition_array)) < 0)
            return nullptr;

        /* Secondary header and partition array read*/
        return gpt;
    }

    ALOGE("Sanity check failed for both headers.");
    errno = EIO;

    return nullptr;
}

bool PartitionTables::gpt_sanity_check()
{
    gpt_header_t tmp_hdr;

    if (header.signature != GPT_HDR_SIGNATURE)
        return false;

    if (header.hdr_size != 92)
        return false;

    /*
     * Calculate header's CRC32 and compare against the CRC32 read from disk
     */
    memcpy(&tmp_hdr, &header, header.hdr_size);
    tmp_hdr.hdr_crc32 = 0;
    if (header.hdr_crc32 != calculate_crc32(&tmp_hdr, tmp_hdr.hdr_size))
        return false;

    return true;
}

int PartitionTables::getIndexForSlottedBootPartition(unsigned slot,
                                             uint32_t& partition_index) const
{
    unsigned i;
    const char16_t* boot_partition_for_slot = slot == 0 ? u"boot_a" : u"boot_b";

    assert(slot <= MAX_SLOTS);

    for (i = 0; i < header.num_parts; i++) {
        if (0 == strcmp16(partition_array[i].name, boot_partition_for_slot)) {
                partition_index = i;
                return 0;
            }
    }

    ALOGV("getIndexForSlottedBootPartition: partition %s does not exist",
                boot_partition_for_slot);

    return -EINVAL;
}

}; //qcom_boot_control

