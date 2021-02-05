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
 * write 1 sector.  data is "size" long and the reset of the sector will be padded to zero
 */
void blockutil_write_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size);

/*
 * read 1 sector. "data" is at least as big as sector_size, size is the number of bytes to read
 */
void blockutil_read_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size);

#endif