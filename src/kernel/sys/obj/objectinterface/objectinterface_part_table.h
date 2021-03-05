//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all partition tables (MBR, GUID) will implement
 */
#ifndef _OBJECTTYPE_PART_TABLE_H
#define _OBJECTTYPE_PART_TABLE_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef uint8_t (*part_table_total_partitions_function)(struct object* obj);
typedef uint64_t (*part_table_get_partition_lba_function)(struct object* obj, uint8_t partition);
// partition type 64 byte string
typedef void (*part_table_get_partition_type_function)(struct object* obj, uint8_t partition, uint8_t* parititon_type,
                                                       uint16_t len);
typedef uint64_t (*part_table_get_sector_count_function)(struct object* obj, uint8_t partition);

/*
 * returns total bytes read
 */
typedef uint32_t (*part_read_sectors_function)(struct object* obj, uint8_t partition_index, uint8_t* data,
                                               uint32_t data_size, uint32_t start_lba);
/*
 * returns total bytes written
 */
typedef uint32_t (*part_write_sectors_function)(struct object* obj, uint8_t partition_index, uint8_t* data,
                                                uint32_t data_size, uint32_t start_lba);

// return 1 if we are ok to detach this device
typedef uint8_t (*part_table_detachable_function)(struct object* obj);

typedef uint16_t (*part_table_sector_size_function)(struct object* obj);

struct objectinterface_part_table {
    part_table_total_partitions_function partitions;
    part_table_get_partition_lba_function lba;
    part_table_get_partition_type_function type;
    part_table_get_sector_count_function sectors;
    part_table_detachable_function detachable;
    part_read_sectors_function read;
    part_write_sectors_function write;
    part_table_sector_size_function sector_size;
};

#endif