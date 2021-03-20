//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* tga image
*/

// https://wiki.osdev.org/Loading_Icons

#ifndef _TGA_H
#define _TGA_H

#include <types.h>

struct tga {
    uint8_t* buffer;
    struct tga_header* header;
    uint32_t* data;
};

struct tga_header {
    uint8_t magic1;              // must be zero
    uint8_t colormap;            // must be zero
    uint8_t encoding;            // must be 2
    uint16_t cmaporig, cmaplen;  // must be zero
    uint8_t cmapent;             // must be zero
    uint16_t x;                  // must be zero
    uint16_t y;                  // image's height
    uint16_t w;                  // image's height
    uint16_t h;                  // image's width
    uint8_t bpp;                 // must be 32
    uint8_t pixeltype;           // must be 40
} __attribute__((packed));

struct tga* tga_load(uint8_t* devname, uint8_t* filename);
void tga_delete(struct tga* targa);
void tga_dump(struct tga* targa);

#endif
