//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <sys/string/mem.h>

uint32_t blockutil_get_sector_count(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT(1 == blockutil_is_block_object(obj));

    uint32_t total_size = blockutil_get_total_size(obj);
    uint32_t sector_size = blockutil_get_sector_size(obj);
    return (total_size / sector_size);
}

uint32_t blockutil_get_sector_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT(1 == blockutil_is_block_object(obj));

    struct objectinterface_block* block_api = (struct objectinterface_block*)obj->api;
    ASSERT_NOT_NULL(block_api);
    ASSERT_NOT_NULL(block_api->sector_size);

    return (*block_api->sector_size)(obj);
}

uint32_t blockutil_get_total_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT(1 == blockutil_is_block_object(obj));

    struct objectinterface_block* block_api = (struct objectinterface_block*)obj->api;
    ASSERT_NOT_NULL(block_api);
    ASSERT_NOT_NULL(block_api->total_size);

    return (*block_api->total_size)(obj);
}

/*
 * write multiple sectors
 */
uint32_t blockutil_write(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba,
                         uint32_t start_byte) {

    //   kprintf("blockutil_write device %s, data_size %llu, start_lba %llu\n", obj->name, data_size, start_lba);

    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT(data_size > 0);

    // check the device type
    ASSERT(1 == blockutil_is_block_object(obj));

    // check that start sector is reasonable
    uint32_t sector_count = blockutil_get_sector_count(obj);
    ASSERT(sector_count > 0);
    ASSERT(start_lba < sector_count);

    // check that end sector is reasonable
    uint32_t sector_size = blockutil_get_sector_size(obj);
    ASSERT(sector_size > 0);
    ASSERT(sector_size > start_byte);

    uint32_t total_sectors = (data_size + start_byte) / sector_size;
    if (0 != (data_size + start_byte) % sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < sector_count);

    // get the api
    struct objectinterface_block* block_api = (struct objectinterface_block*)obj->api;
    ASSERT_NOT_NULL(block_api);
    if (0 != block_api->write) {

        // loop over lbas, reading a sector at a time and copying into the data
        uint32_t total_bytes_written = 0;
        for (uint32_t i = 0; i < total_sectors; i++) {
            uint8_t buffer[sector_size];
            memzero(buffer, sector_size);
            if (i == 0) {
                uint32_t needed = sector_size - start_byte;
                if (needed > data_size) {
                    needed = data_size;
                }
                //      kprintf("needed %llu\n", needed);
                memcpy(&(buffer[start_byte]), &(data[total_bytes_written]), needed);
                total_bytes_written += needed;
            } else if (i == total_sectors - 1) {
                uint32_t offset = data_size - total_bytes_written;
                memcpy(buffer, &(data[total_bytes_written]), offset);
                total_bytes_written += offset;
            } else {
                memcpy(buffer, &(data[total_bytes_written]), sector_size);
                total_bytes_written += sector_size;
            }
            uint32_t written = (*block_api->write)(obj, buffer, sector_size, i + start_lba);
            ASSERT(written == sector_size);
        }

        // done
        return total_bytes_written;
    } else {
        // if the write API is nt provided, then the block_device is read-only.  return 0 bytes written.
        return 0;
    }
}

/*
 * read multiple sectors
 */
uint32_t blockutil_read(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba,
                        uint32_t start_byte) {

    //  kprintf("blockutil_read device %s, data_size %llu, start_lba %llu\n", obj->name, data_size, start_lba);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT(data_size > 0);

    // check the device type
    ASSERT(1 == blockutil_is_block_object(obj));

    // check that start sector is reasonable
    uint32_t sector_count = blockutil_get_sector_count(obj);
    ASSERT(sector_count > 0);
    ASSERT(start_lba < sector_count);

    // check that end sector is reasonable
    uint32_t sector_size = blockutil_get_sector_size(obj);
    ASSERT(sector_size > 0);
    ASSERT(sector_size > start_byte);
    uint32_t total_sectors = (data_size + start_byte) / sector_size;
    if (0 != (data_size + start_byte) % sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < sector_count);

    // get the api
    struct objectinterface_block* block_api = (struct objectinterface_block*)obj->api;
    ASSERT_NOT_NULL(block_api);
    ASSERT_NOT_NULL(block_api->read);

    // loop over lbas, reading a sector at a time and copying into the data
    uint32_t total_bytes_copied = 0;
    for (uint32_t i = 0; i < total_sectors; i++) {
        uint8_t buffer[sector_size];
        memzero(buffer, sector_size);
        uint32_t read = (*block_api->read)(obj, buffer, sector_size, i + start_lba);
        ASSERT(read == sector_size);
        if (i == 0) {
            uint32_t needed = sector_size - start_byte;
            if (needed > data_size) {
                needed = data_size;
            }
            //      kprintf("needed %llu\n", needed);
            memcpy(&(data[total_bytes_copied]), &(buffer[start_byte]), needed);
            total_bytes_copied += needed;
        } else if (i == total_sectors - 1) {
            uint32_t offset = data_size - total_bytes_copied;
            //      kprintf("offset %llu\n", offset);
            memcpy(&(data[total_bytes_copied]), buffer, offset);
            total_bytes_copied += offset;
        } else {
            memcpy(&(data[total_bytes_copied]), buffer, sector_size);
            total_bytes_copied += sector_size;
        }
    }

    // done
    return total_bytes_copied;
}

uint8_t blockutil_is_block_object(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    if ((obj->objectype == DISK) || (obj->objectype == VBLOCK) || (obj->objectype == RAMDISK) ||
        (obj->objectype == PARTITION)) {
        return 1;
    }
    return 0;
}