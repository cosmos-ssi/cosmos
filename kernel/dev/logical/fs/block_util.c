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
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>

uint32_t blockutil_get_sector_count(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(1 == blockutil_is_block_device(dev));

    uint32_t total_size = blockutil_get_total_size(dev);
    uint32_t sector_size = blockutil_get_sector_size(dev);
    return (total_size / sector_size);
}

uint32_t blockutil_get_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(1 == blockutil_is_block_device(dev));

    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    return (*block_api->sector_size)(dev);
}

uint32_t blockutil_get_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(1 == blockutil_is_block_device(dev));

    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    return (*block_api->total_size)(dev);
}

/*
 * write multiple sectors
 */
uint32_t blockutil_write_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {

    //    kprintf("blockutil_write_sectors device %s, data_size %llu, start_lba %llu\n", dev->name, data_size, start_lba);

    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    // check the device type
    ASSERT(1 == blockutil_is_block_device(dev));

    // check that start sector is reasonable
    uint32_t sector_count = blockutil_get_sector_count(dev);
    ASSERT(start_lba < sector_count);

    // check that end sector is reasonable
    uint32_t sector_size = blockutil_get_sector_size(dev);
    uint32_t total_sectors = data_size / sector_size;
    if (0 != data_size % sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < sector_count);

    // get the api
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    // make the buffer
    uint8_t buffer[(total_sectors * sector_size)];
    memzero(buffer, (total_sectors * sector_size));
    memcpy(buffer, data, data_size);

    // write
    uint32_t written = (*block_api->write)(dev, buffer, (total_sectors * sector_size), start_lba);
    ASSERT(written == (total_sectors * sector_size));

    // done
    return data_size;
}

/*
 * read multiple sectors
 */
uint32_t blockutil_read_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {

    //    kprintf("blockutil_read_sectors device %s, data_size %llu, start_lba %llu\n", dev->name, data_size, start_lba);

    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    // check the device type
    ASSERT(1 == blockutil_is_block_device(dev));

    // check that start sector is reasonable
    uint32_t sector_count = blockutil_get_sector_count(dev);
    ASSERT(start_lba < sector_count);

    // check that end sector is reasonable
    uint32_t sector_size = blockutil_get_sector_size(dev);
    uint32_t total_sectors = data_size / sector_size;
    if (0 != data_size % sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < sector_count);

    // get the api
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    // make the buffer
    uint8_t buffer[(total_sectors * sector_size)];
    memzero(buffer, (total_sectors * sector_size));

    // read
    uint32_t read = (*block_api->read)(dev, buffer, (total_sectors * sector_size), start_lba);
    ASSERT(read == (total_sectors * sector_size));

    // copy
    memcpy(data, buffer, data_size);

    // done
    return data_size;
}

uint8_t blockutil_is_block_device(struct device* dev) {
    if ((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
        (dev->devicetype == PARTITION)) {
        return 1;
    }
    return 0;
}