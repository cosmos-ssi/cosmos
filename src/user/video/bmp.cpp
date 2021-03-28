//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <assert.hpp>
#include <bmp.hpp>
#include <malloc.h>
#include <object/objects.hpp>

bmp::bmp(uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);

    uint32_t len;
    uint8_t* data = FilesystemObject::read_file(devname, filename, &len);
    //  kprintf("bmp size %llu\n", len);

    this->buffer = data;
    this->buffer_size = len;
    this->file_header = (struct bitmap_file_header*)data;
    this->info_header = (struct bitmap_info_header*)&(data[sizeof(struct bitmap_file_header)]);
    this->bitdata = (uint32_t*)&(data[this->file_header->offset]);
}

bmp::~bmp() {
    if (0 != this->buffer) {
        free(this->buffer);
    }
}
