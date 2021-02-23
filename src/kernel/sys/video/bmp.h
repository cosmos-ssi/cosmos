//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* bitmap image
*/
#ifndef _BMP_H
#define _BMP_H

#include <types.h>

struct bmp {
    uint8_t* buffer;
    uint32_t* bitdata;
    uint32_t buffer_size;
    uint32_t offset;
    struct bitmap_file_header* file_header;
    struct bitmap_info_header* info_header;
};

struct bitmap_file_header {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} __attribute__((packed));

struct bitmap_info_header {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t size_image;
    uint32_t x_pix_per_meter;
    uint32_t y_pix_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
};

struct palette {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct bmp* bmp_load(uint8_t* devname, uint8_t* filename);
void bmp_delete(struct bmp* bitmap);

#endif
