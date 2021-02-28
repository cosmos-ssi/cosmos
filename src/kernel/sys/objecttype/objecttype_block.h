//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all ATA devices will implement
 */
#ifndef _OBJECTTYPE_BLOCK_H
#define _OBJECTTYPE_BLOCK_H

#include <sys/objectmgr/objectmgr.h>

#include <types.h>
/*
* read bytes into 'data'.  'data_size' is the number of bytes to read and 'start_lba' is the starting lba. 
* return total bytes read
*/
typedef uint32_t (*block_read_sectors_function)(struct object* obj, uint8_t* data, uint32_t data_size,
                                                uint32_t start_lba);
/*
* write bytes from 'data'.  'data_size' is the number of bytes to write and 'start_lba' is the starting lba.
* return total bytes written
*/
typedef uint32_t (*block_write_sectors_function)(struct object* obj, uint8_t* data, uint32_t data_size,
                                                 uint32_t start_lba);
typedef uint16_t (*block_sector_size_function)(struct object* obj);
typedef uint32_t (*block_total_size_function)(struct object* obj);

struct objecttype_block {
    block_read_sectors_function read;
    block_write_sectors_function write;
    block_sector_size_function sector_size;
    block_total_size_function total_size;
};

#endif