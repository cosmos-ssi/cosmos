//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_UTIL_H
#define _BLOCK_UTIL_H

#include <sys/objecttype/objecttype_block.h>
#include <types.h>

uint32_t blockutil_get_sector_size(struct object* obj);
uint32_t blockutil_get_sector_count(struct object* obj);
uint32_t blockutil_get_total_size(struct object* obj);

/*
* read bytes into 'data'.  'data_size' is the number of bytes to read and 'start_lba' is the starting lba. 
* return total bytes read
*/
/*
* if data_size smaller than the number of bytes in the sectors written multipled by sector size, the 
* remaining space is null padded. data writng starts at "start_byte" bytes into first sector.
*/
uint32_t blockutil_write(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba,
                         uint32_t start_byte);
/*
* write bytes from 'data'.  'data_size' is the number of bytes to write and 'start_lba' is the starting lba.
* return total bytes written
*/
/*
* the total data read from the block device is sectors * sector size, which may be larger than data_size
* only data_size bytes will be written to data.  data reading starts from "start_byte" bytes into first sector.
*/
uint32_t blockutil_read(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba, uint32_t start_byte);
/*
* check if a device is a block device (this is, supports deviceapi_block)
*/
uint8_t blockutil_is_block_object(struct object* obj);
#endif