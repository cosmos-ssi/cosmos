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
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/video/psf.h>

const uint8_t PSF1_MAGIC[2] = {0x36, 0x04};

struct psf1_font* psf_load(uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);

    uint32_t len = 0;
    uint8_t* buffer = file_util_read_file(devname, filename, &len);
    if ((buffer[0] == PSF1_MAGIC[0]) && (buffer[1] == PSF1_MAGIC[1])) {
        return (struct psf1_font*)buffer;
    } else {
        return 0;
    }
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

uint32_t psf_total_characters(struct psf1_font* font) {
    ASSERT_NOT_NULL(font);
    if ((font->filemode == 0) || (font->filemode == 2)) {
        return 256;
    } else {
        return 512;
    }
}

uint32_t psf_height(struct psf1_font* font) {
    ASSERT_NOT_NULL(font);
    return font->fontheight;
}

uint8_t psf_is_unicode(struct psf1_font* font) {
    ASSERT_NOT_NULL(font);
    if ((font->filemode == 2) || (font->filemode == 3)) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t* psf_character(struct psf1_font* font, uint16_t idx) {
    ASSERT_NOT_NULL(font);
    ASSERT_NOT_NULL(idx <= psf_total_characters(font));
    uint32_t offset = sizeof(struct psf1_font) + (idx * psf_height(font));
    return &(((uint8_t*)font)[offset]);
}
