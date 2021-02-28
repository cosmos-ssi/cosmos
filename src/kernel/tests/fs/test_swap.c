//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/swap/swap.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_swap.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/fs/ramdisk_helper.h>
#include <types.h>

const uint8_t SWAP_TEST_STRING[] = "We were very tired and we were very merry";

void test_swap() {
    kprintf("Testing Swap\n");

    // make rd
    struct object* rd = ramdisk_helper_create_rd();
    ASSERT_NOT_NULL(rd);

    // attach the swap
    struct object* swap_device = swap_attach(rd);
    ASSERT_NOT_NULL(swap_device);

    struct objectinterface_swap* swap_api = (struct objectinterface_swap*)swap_device->api;
    ASSERT_NOT_NULL(swap_api);

    uint16_t block_size = (*swap_api->block_size)(swap_device);
    // size of the swap blocks is the block size of the underlying device
    ASSERT(RAMDISK_SECTOR_SIZE == block_size);

    // write the string
    uint8_t write_buffer[block_size];
    memzero(write_buffer, block_size);
    strncpy(write_buffer, SWAP_TEST_STRING, strlen(SWAP_TEST_STRING) + 1);

    (*swap_api->write)(swap_device, write_buffer, 0);

    // read it back
    uint8_t read_buffer[block_size];
    memzero(read_buffer, block_size);

    (*swap_api->read)(swap_device, read_buffer, 0);

    ASSERT(strlen(SWAP_TEST_STRING) == strlen(read_buffer));
    ASSERT(0 == strcmp(SWAP_TEST_STRING, read_buffer));

    // detach the swap
    swap_detach(swap_device);

    // remove rd
    ramdisk_helper_remove_rd(rd);
}
