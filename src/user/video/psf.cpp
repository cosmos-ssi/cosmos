//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}
#include <assert.hpp>
#include <object/objects.hpp>
#include <psf.hpp>

const uint8_t PSF1_MAGIC[2] = {0x36, 0x04};

psf1_font::psf1_font(uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);

    uint32_t len = 0;
    uint8_t* buffer = FilesystemObject::read_file(devname, filename, &len);
    if ((buffer[0] == PSF1_MAGIC[0]) && (buffer[1] == PSF1_MAGIC[1])) {
        this->data = (struct psf1_font_data*)buffer;
    } else {
        this->data = 0;
    }
}

psf1_font::~psf1_font() {
    ASSERT_NOT_NULL(this->data);
    free(this->data);
}

void psf1_font::dump() {
    ASSERT_NOT_NULL(this->data);
    //  kprintf("psf magic[0] %#llX\n", font->magic[0]);
    //  kprintf("psf magic[1] %#llX\n", font->magic[1]);
    //  kprintf("psf filemode %#llX\n", font->filemode);
    // kprintf("psf fontheight %#llX\n", font->fontheight);
}

uint32_t psf1_font::total_characters() {
    ASSERT_NOT_NULL(this->data);
    if ((this->data->filemode == 0) || (this->data->filemode == 2)) {
        return 256;
    } else {
        return 512;
    }
}

uint32_t psf1_font::height() {
    ASSERT_NOT_NULL(this->data);
    return this->data->fontheight;
}

uint8_t psf1_font::is_unicode() {
    ASSERT_NOT_NULL(this->data);
    if ((this->data->filemode == 2) || (this->data->filemode == 3)) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t* psf1_font::character(uint16_t idx) {
    ASSERT_NOT_NULL(this->data);
    ASSERT(idx <= total_characters());
    uint32_t offset = sizeof(struct psf1_font) + (idx * height());
    return &(((uint8_t*)this->data)[offset]);
}
