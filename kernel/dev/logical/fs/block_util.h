//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_UTIL_H
#define _BLOCK_UTIL_H

#include <sys/deviceapi/deviceapi_block.h>
#include <types.h>

uint32_t blockutil_get_sector_size(struct device* dev);
uint32_t blockutil_get_sector_count(struct device* dev);
uint32_t blockutil_get_total_size(struct device* dev);

/*
* read bytes into 'data'.  'data_size' is the number of bytes to read and 'start_lba' is the starting lba. 
* return total bytes read
*/
uint32_t blockutil_write_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba);
/*
* write bytes from 'data'.  'data_size' is the number of bytes to write and 'start_lba' is the starting lba.
* return total bytes written
*/
uint32_t blockutil_read_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba);
/*
* check if a device is a block device (this is, supports deviceapi_block)
*/
uint8_t blockutil_is_block_device(struct device* dev);
#endif