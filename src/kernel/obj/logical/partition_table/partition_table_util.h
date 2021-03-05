//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PARTITION_TABLE_UTIL_H
#define _PARTITION_TABLE_UTIL_H

#include <types.h>

struct object;

uint16_t partition_table_util_sector_size(struct object* partition_table_object, uint8_t partition_index);
uint32_t partition_table_util_total_size(struct object* partition_table_object, uint8_t partition_index);

uint32_t partition_table_util_write_sectors(struct object* partition_table_object, uint8_t partition_index,
                                            uint8_t* data, uint32_t data_size, uint32_t start_lba);
uint32_t partition_table_util_read_sectors(struct object* partition_table_object, uint8_t partition_index,
                                           uint8_t* data, uint32_t data_size, uint32_t start_lba);

#endif