//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_blockdevice.h>

uint8_t* testdata = "We were very tired, we were very merry, \
We had gone back and forth all night on the ferry. \
We hailed, Good morrow, mother! to a shawl-covered head, \
And bought a morning paper, which neither of us read; \
And she wept, God bless you! for the apples and pears, \
And we gave her all our money but our subway fares.";

void test_block_device_base_api(struct object* dev) {
    struct objecttype_block* block_api = (struct objecttype_block*)dev->api;
    ASSERT_NOT_NULL(block_api);
    uint32_t s = strlen(testdata);
    uint32_t written = (*block_api->write)(dev, testdata, s + 1, 7);
    ASSERT(written == s + 1);
    uint8_t readdata[s + 1];
    memzero((uint8_t*)readdata, s + 1);
    uint32_t read = (*block_api->read)(dev, readdata, s + 1, 7);
    kprintf("read %llu\n", read);
    ASSERT(read == s + 1);
    ASSERT(0 == strcmp(readdata, testdata));
    ASSERT(strlen(readdata) == strlen(testdata));
}

void test_blockutil(struct object* dev) {
    uint32_t s = strlen(testdata);
    uint32_t written = blockutil_write(dev, testdata, s + 1, 0, 0);
    ASSERT(written == s + 1);
    uint8_t buffer[s + 1];
    uint32_t read = blockutil_read(dev, (uint8_t*)buffer, s + 1, 0, 0);
    ASSERT(read == s + 1);
    ASSERT(s == strlen(buffer));
    ASSERT(0 == strcmp(buffer, testdata));
}

void test_block_device(struct object* dev) {
    kprintf("Testing block device %s\n", dev->name);
    //   test_block_device_base_api(dev);
    test_blockutil(dev);
}
