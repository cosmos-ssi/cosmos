//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* BDF font
*/
// https://wiki.osdev.org/VGA_Fonts#Decoding_of_bitmap_fonts

#ifndef _BDF_H
#define _BDF_H

#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

struct bdf_glyph {
    uint8_t name[16];
    uint8_t data[12];
};

struct bdf {
    struct arraylist* glyphs;
};

struct bdf* bdf_new();
void bdf_load(struct bdf* font, uint8_t* devname, uint8_t* filename);
void bdf_delete(struct bdf* font);

#endif
