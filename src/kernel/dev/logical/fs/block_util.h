//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_UTIL_H
#define _BLOCK_UTIL_H

#include <types.h>

struct device;

uint32_t blockutil_get_sector_size(struct device* dev);
uint32_t blockutil_get_sector_count(struct device* dev);
uint32_t blockutil_get_total_size(struct device* dev);

/*
* read bytes into 'data'.  'data_size' is the number of bytes to read and 'start_lba' is the starting lba. 
* return total bytes read
*/
/*
* if data_size smaller than the number of bytes in the sectors written multipled by sector size, the 
* remaining space is null padded
*/
uint32_t blockutil_write(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba);
/*
* write bytes from 'data'.  'data_size' is the number of bytes to write and 'start_lba' is the starting lba.
* return total bytes written
*/
/*
* the total data read from the block device is sectors * sector size, which may be larger than data_size
* only data_size bytes will be written to data
*/
uint32_t blockutil_read(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba);
/*
* check if a device is a block device (this is, supports deviceapi_block)
*/
uint8_t blockutil_is_block_device(struct device* dev);
#endif