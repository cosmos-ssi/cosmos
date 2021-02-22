//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/gui/psf.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct psf1_font* psf_load(uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);

    uint32_t len = 0;
    uint8_t* buffer = file_util_read_file(devname, filename, &len);
    //   debug_show_memblock(buffer, len);
    kprintf("psf size %llu\n", len);
    return (struct psf1_font*)buffer;
}

void psf_delete(struct psf1_font* font) {
    ASSERT_NOT_NULL(font);
    kfree(font);
}

void psf_dump(struct psf1_font* font) {
    ASSERT_NOT_NULL(font);
    kprintf("psf magic[0] %#llX\n", font->magic[0]);
    kprintf("psf magic[1] %#llX\n", font->magic[1]);
    kprintf("psf filemode %#llX\n", font->filemode);
    kprintf("psf fontheight %#llX\n", font->fontheight);
}
