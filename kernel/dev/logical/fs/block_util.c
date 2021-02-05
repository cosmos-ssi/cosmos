//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/string/mem.h>

uint32_t blockutil_get_sector_count(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
           (dev->devicetype == PARTITION));

    uint32_t total_size = blockutil_get_total_size(dev);
    uint32_t sector_size = blockutil_get_sector_size(dev);
    return total_size / sector_size;
}

uint32_t blockutil_get_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
           (dev->devicetype == PARTITION));

    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    return (*block_api->sector_size)(dev);
}

uint32_t blockutil_get_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
           (dev->devicetype == PARTITION));

    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    return (*block_api->total_size)(dev);
}

/*
 * write 1 sector.  data is "size" long and the reset of the sector will be padded to zero
 */
void blockutil_write_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
           (dev->devicetype == PARTITION));
    ASSERT_NOT_NULL(data);
    uint16_t sector_size = blockutil_get_sector_size(dev);
    ASSERT(size <= sector_size);
    ASSERT(sector <= blockutil_get_sector_count(dev));
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    // make the buffer
    uint8_t buffer[sector_size];
    memzero(buffer, sector_size);
    memcpy(buffer, data, size);

    // write
    (*block_api->write)(dev, sector, buffer, 1);
}

/*
 * read 1 sector. "data" is at least as big as sector_size, size is the number of bytes to read
 */
void blockutil_read_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
           (dev->devicetype == PARTITION));
    ASSERT_NOT_NULL(data);
    uint16_t sector_size = blockutil_get_sector_size(dev);
    ASSERT(size <= sector_size);
    ASSERT(sector <= blockutil_get_sector_count(dev));
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    // make the buffer
    uint8_t buffer[sector_size];
    memzero(buffer, sector_size);

    // read
    (*block_api->read)(dev, sector, buffer, 1);

    // copy
    memcpy(data, buffer, size);
}