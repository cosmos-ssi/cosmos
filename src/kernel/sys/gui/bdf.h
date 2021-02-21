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

#include <types.h>

struct bdf {
    uint8_t* buffer;
    uint32_t buffer_size;
};

struct bdf* bdf_new();
void bdf_load(struct bdf* font, uint8_t* devname, uint8_t* filename);
void bdf_delete(struct bdf* font);

#endif
