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
    uint32_t buffer_size;
};

struct bmp* bmp_load(uint8_t* devname, uint8_t* filename);
void bmp_delete(struct bmp* bitmap);

#endif
