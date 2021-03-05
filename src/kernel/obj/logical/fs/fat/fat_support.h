//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FAT_SUPPORT_H
#define _FAT_SUPPORT_H

#include <types.h>

struct object;

// https://wiki.osdev.org/FAT

// Extended fat32 stuff
struct fat_extBS_32 {
    uint32_t table_size_32;
    uint16_t extended_flags;
    uint16_t fat_version;
    uint32_t root_cluster;
    uint16_t fat_info;
    uint16_t backup_BS_sector;
    uint8_t reserved_0[12];
    uint8_t drive_number;
    uint8_t reserved_1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fat_type_label[8];
} __attribute__((packed));

// Extended fat12 and fat16 stuff
struct fat_extBS_16 {
    uint8_t bios_drive_num;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fat_type_label[8];
} __attribute__((packed));

struct fat_BS {
    uint8_t bootjmp[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t table_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_32;

    // this will be cast to it's specific type once the driver actually knows what type of FAT this is.
    uint8_t extended_section[54];
} __attribute__((packed));

struct fat_dir {
    uint8_t name[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time;
    uint16_t time;
    uint16_t date;
    uint16_t last_accessed;
    uint16_t cluster_high;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t cluster_low;
    uint32_t size;
} __attribute__((packed));

enum fat_type { FAT12 = 0x01, FAT16 = 0x02, FAT32 = 0x03, ExFAT = 0x04 };

/*
 * calculated parameters of this file system
 */
struct fat_fs_parameters {
    // from fs_BS
    uint8_t sectors_per_cluster;
    // from device
    uint16_t sector_size;
    // from device
    uint32_t total_size;
    /*
     * rest of these are calculated
     */
    uint32_t total_sectors;
    uint32_t fat_size;
    uint32_t root_dir_sectors;
    uint32_t first_data_sector;
    uint32_t first_fat_sector;
    uint32_t data_sectors;
    uint32_t total_clusters;
    uint32_t first_root_dir_sector;
    uint32_t root_cluster_32;
    enum fat_type type;
};

void fat_dump_fat_fs_parameters(struct fat_fs_parameters* param);
void fat_dump_fat_extBS_16(struct fat_extBS_16* ebs);
void fat_read_fs_parameters(struct object* obj, struct fat_fs_parameters* param);
uint64_t fat_first_sector_of_cluster(uint32_t cluster, struct fat_fs_parameters* fs_parameters);
uint32_t fat_fat12_next_cluster(struct object* obj, uint32_t current_cluster, struct fat_fs_parameters* fs_parameters);
uint32_t fat_fat16_next_cluster(struct object* obj, uint32_t current_cluster, struct fat_fs_parameters* fs_parameters);

#endif
