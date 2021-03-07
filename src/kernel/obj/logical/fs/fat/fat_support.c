//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fat/fat_support.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

void fat_dump_fat_fs_parameters(struct fat_fs_parameters* param) {
    kprintf("  sector size: %llu\n", param->sector_size);
    kprintf("  total size: %llu\n", param->total_size);
    kprintf("  total_sectors %llu\n", param->total_sectors);
    kprintf("  fat_size %llu\n", param->fat_size);
    kprintf("  root_dir_sectors %llu\n", param->root_dir_sectors);
    kprintf("  first_data_sector %llu\n", param->first_data_sector);
    kprintf("  first_fat_sector %llu\n", param->first_fat_sector);
    kprintf("  data_sectors %llu\n", param->data_sectors);
    kprintf("  total_clusters %llu\n", param->total_clusters);
    kprintf("  first_root_dir_sector %llu\n", param->first_root_dir_sector);
    kprintf("  root_cluster_32 %llu\n", param->root_cluster_32);
    kprintf("  type: %llu\n", param->type);
    kprintf("  sectors_per_cluster: %llu\n", param->sectors_per_cluster);
}

void fat_dump_fat_extBS_16(struct fat_extBS_16* ebs) {
    kprintf("drive_number %llu\n", ebs->bios_drive_num);
    //  kprintf("volume_id %llu\n", ebs->volume_id);
    kprintf("volume_label %s\n", ebs->volume_label);
    //  kprintf("fat_type_label %s \n", ebs->fat_type_label);

    //   debug_show_memblock((uint8_t*)ebs, sizeof(struct fat_extBS_16));
}

void fat_read_fs_parameters(struct object* obj, struct fat_fs_parameters* param) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(param);
    ASSERT(1 == blockutil_is_block_object(obj));
    param->sector_size = blockutil_get_sector_size(obj);
    param->total_size = blockutil_get_total_size(obj);
    uint8_t* buffer = kmalloc(param->sector_size);
    memset(buffer, 0, param->sector_size);

    blockutil_read(obj, buffer, param->sector_size, 0, 0);

    struct fat_BS* fat_boot = (struct fat_BS*)buffer;
    //   struct fat_extBS_16* fat_boot_ext_16 = (struct fat_extBS_16*)&(fat_boot->extended_section);
    struct fat_extBS_32* fat_boot_ext_32 = (struct fat_extBS_32*)&(fat_boot->extended_section);

    param->sectors_per_cluster = fat_boot->sectors_per_cluster;
    param->total_sectors = (fat_boot->total_sectors_16 == 0) ? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
    param->fat_size = (fat_boot->table_size_16 == 0) ? fat_boot_ext_32->table_size_32 : fat_boot->table_size_16;
    param->root_dir_sectors =
        ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
    param->first_data_sector =
        fat_boot->reserved_sector_count + (fat_boot->table_count * param->fat_size) + param->root_dir_sectors;
    param->first_fat_sector = fat_boot->reserved_sector_count;
    param->data_sectors =
        fat_boot->total_sectors_16 -
        (fat_boot->reserved_sector_count + (fat_boot->table_count * param->fat_size) + param->root_dir_sectors);
    param->total_clusters = param->data_sectors / fat_boot->sectors_per_cluster;
    param->first_root_dir_sector = param->first_data_sector - param->root_dir_sectors;
    param->root_cluster_32 = fat_boot_ext_32->root_cluster;

    // fat type
    if (param->total_clusters < 4085) {
        param->type = FAT12;
    } else if (param->total_clusters < 65525) {
        param->type = FAT16;
    } else if (param->total_clusters < 268435445) {
        param->type = FAT32;
    } else {
        param->type = ExFAT;
    }
    kfree(buffer);
}

void fat_filename_from_fat(uint8_t* fat_name, uint8_t* normal_name, uint16_t size) {
    ASSERT_NOT_NULL(fat_name);
    ASSERT_NOT_NULL(normal_name);
    ASSERT(size >= 11);
    uint8_t j = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (fat_name[i] != ' ') {
            normal_name[j++] = fat_name[i];
        }
    }
    if ((' ' != fat_name[8]) && (' ' != fat_name[9]) && (' ' != fat_name[10])) {
        normal_name[j++] = '.';
        normal_name[j++] = fat_name[8];
        normal_name[j++] = fat_name[9];
        normal_name[j++] = fat_name[10];
    }
    normal_name[j] = 0;
}

void fat_filename_to_fat(uint8_t* normal_name, uint8_t* fat_name, uint16_t size) {
    PANIC("not implemented");
}
/*
 * find first sector of cluster
 */
uint64_t fat_first_sector_of_cluster(uint32_t cluster, struct fat_fs_parameters* fs_parameters) {
    return ((cluster - 2) * fs_parameters->sectors_per_cluster) + fs_parameters->first_data_sector;
}

uint32_t fat_fat12_next_cluster(struct object* obj, uint32_t current_cluster, struct fat_fs_parameters* fs_parameters) {
    uint8_t FAT_table[fs_parameters->sector_size];
    uint32_t fat_offset = current_cluster + (current_cluster / 2);  // multiply by 1.5
    kprintf("fat_offset %llu\n", fat_offset);

    uint32_t fat_sector = fs_parameters->first_fat_sector + (fat_offset / fs_parameters->sector_size);
    kprintf("fat_sector %llu\n", fat_sector);

    uint32_t ent_offset = fat_offset % fs_parameters->sector_size;

    memset((uint8_t*)&FAT_table, 0, fs_parameters->sector_size);
    blockutil_read(obj, (uint8_t*)&FAT_table, fs_parameters->sector_size, fat_sector, 0);

    unsigned short table_value = *(unsigned short*)&FAT_table[ent_offset];

    if (current_cluster & 0x0001) {
        table_value = table_value >> 4;
    } else {
        table_value = table_value & 0x0FFF;
    }
    return table_value;
}

uint32_t fat_fat16_next_cluster(struct object* obj, uint32_t current_cluster, struct fat_fs_parameters* fs_parameters) {
    uint8_t FAT_table[fs_parameters->sector_size];
    uint32_t fat_offset = current_cluster * 2;
    uint32_t fat_sector = fs_parameters->first_fat_sector + (fat_offset / fs_parameters->sector_size);
    uint32_t ent_offset = fat_offset % fs_parameters->sector_size;

    memset((uint8_t*)&FAT_table, 0, fs_parameters->sector_size);
    blockutil_read(obj, (uint8_t*)&FAT_table, fs_parameters->sector_size, fat_sector, 0);

    return *(unsigned short*)&FAT_table[ent_offset];
}
