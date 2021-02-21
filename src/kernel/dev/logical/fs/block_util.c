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
#include <sys/kmalloc/kmalloc.h>
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
    ASSERT_NOT_NULL(block_api->sector_size);

    return (*block_api->sector_size)(dev);
}

uint32_t blockutil_get_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(1 == blockutil_is_block_device(dev));

    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);
    ASSERT_NOT_NULL(block_api->total_size);

    return (*block_api->total_size)(dev);
}

/*
 * write multiple sectors
 */
uint32_t blockutil_write(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {

    //   kprintf("blockutil_write device %s, data_size %llu, start_lba %llu\n", dev->name, data_size, start_lba);

    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT(data_size > 0);

    // check the device type
    ASSERT(1 == blockutil_is_block_device(dev));

    // check that start sector is reasonable
    uint32_t sector_count = blockutil_get_sector_count(dev);
    ASSERT(sector_count > 0);
    ASSERT(start_lba < sector_count);

    // check that end sector is reasonable
    uint32_t sector_size = blockutil_get_sector_size(dev);
    ASSERT(sector_size > 0);

    uint32_t total_sectors = data_size / sector_size;
    if (0 != data_size % sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < sector_count);

    // get the api
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);
    if (0 != block_api->write) {
        // make the buffer
        uint32_t buffer_size = total_sectors * sector_size;
        uint8_t* buffer = kmalloc(buffer_size);
        memzero(buffer, (buffer_size));
        memcpy(buffer, data, data_size);

        // write
        uint32_t written = (*block_api->write)(dev, buffer, buffer_size, start_lba);
        ASSERT(written == buffer_size);

        // done w buffer
        kfree(buffer);

        // done
        return data_size;
    } else {
        // if the write API is nt provided, then the block_device is read-only.  return 0 bytes written.
        return 0;
    }
}

/*
 * read multiple sectors
 */
uint32_t blockutil_read(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {

    //   kprintf("blockutil_read device %s, data_size %llu, start_lba %llu\n", dev->name, data_size, start_lba);

    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT(data_size > 0);

    // check the device type
    ASSERT(1 == blockutil_is_block_device(dev));

    // check that start sector is reasonable
    uint32_t sector_count = blockutil_get_sector_count(dev);
    ASSERT(sector_count > 0);
    ASSERT(start_lba < sector_count);

    // check that end sector is reasonable
    uint32_t sector_size = blockutil_get_sector_size(dev);
    ASSERT(sector_size > 0);
    uint32_t total_sectors = data_size / sector_size;
    if (0 != data_size % sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < sector_count);

    // get the api
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);
    ASSERT_NOT_NULL(block_api->read);

    // loop over lbas, reading a sector at a time and copying into the data
    uint32_t total_bytes_copied = 0;
    for (uint32_t i = 0; i < total_sectors; i++) {
        uint8_t buffer[sector_size];
        memzero(buffer, sector_size);
        uint32_t read = (*block_api->read)(dev, buffer, sector_size, i + start_lba);
        ASSERT(read == sector_size);
        //      kprintf("read bytes %llu index %llu\n", read, i);
        if (i == total_sectors - 1) {
            uint32_t offset = data_size - total_bytes_copied;
            //   kprintf("offset %llu\n", offset);
            memcpy(&(data[i * sector_size]), buffer, offset);
            total_bytes_copied += offset;
        } else {
            memcpy(&(data[i * sector_size]), buffer, sector_size);
            total_bytes_copied += sector_size;
        }
    }

    // done
    return total_bytes_copied;
}

uint8_t blockutil_is_block_device(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    if ((dev->devicetype == DISK) || (dev->devicetype == VBLOCK) || (dev->devicetype == RAMDISK) ||
        (dev->devicetype == PARTITION)) {
        return 1;
    }
    return 0;
}