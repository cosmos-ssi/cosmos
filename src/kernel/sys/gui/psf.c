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

struct psf_font* psf_load(struct psf_font* font, uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);
    ASSERT_NOT_NULL(font);

    uint32_t len = 0;
    uint8_t* buffer = file_util_read_file(devname, filename, &len);

    kprintf("psf size %llu\n", len);
    return (struct psf_font*)buffer;
}

void psf_delete(struct psf_font* font) {
    ASSERT_NOT_NULL(font);
    kfree(font);
}
