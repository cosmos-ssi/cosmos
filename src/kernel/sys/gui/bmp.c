//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/gui/bmp.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct bmp* bmp_new() {
    struct bmp* ret = kmalloc(sizeof(struct bmp));
    memzero((uint8_t*)ret, sizeof(struct bmp));
    return ret;
}

void bmp_load(struct bmp* bitmap, uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);
    ASSERT_NOT_NULL(bitmap);
    /*
    * delete previous bmp if there is one
    */
    if (0 != bitmap->buffer) {
        kfree(bitmap->buffer);
        bitmap->buffer = 0;
        bitmap->buffer_size = 0;
    }

    uint32_t len;
    uint8_t* data = file_util_read_file(devname, filename, &len);
    kprintf("bmp size %llu\n", len);

    bitmap->buffer = data;
    bitmap->buffer_size = len;
}

void bmp_delete(struct bmp* bitmap) {
    ASSERT_NOT_NULL(bitmap);
    if (0 != bitmap->buffer) {
        kfree(bitmap->buffer);
    }
    kfree(bitmap);
}
