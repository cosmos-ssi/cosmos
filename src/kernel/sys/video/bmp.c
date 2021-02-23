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
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/video/bmp.h>

struct bmp* bmp_load(uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);

    struct bmp* ret = (struct bmp*)kmalloc(sizeof(struct bmp));

    uint32_t len;
    uint8_t* data = file_util_read_file(devname, filename, &len);
    //  kprintf("bmp size %llu\n", len);

    ret->buffer = data;
    ret->buffer_size = len;

    ret->file_header = (struct bitmap_file_header*)data;
    ret->info_header = (struct bitmap_info_header*)&(data[sizeof(struct bitmap_file_header)]);
    return ret;
}

void bmp_delete(struct bmp* bitmap) {
    ASSERT_NOT_NULL(bitmap);
    if (0 != bitmap->buffer) {
        kfree(bitmap->buffer);
    }
    kfree(bitmap);
}
