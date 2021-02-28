//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all BGA devices will implement
 */
#ifndef _OBJECTTYPE_BGA_H
#define _OBJECTTYPE_BGA_H

#include <sys/objectmgr/objectmgr.h>

#include <types.h>

struct objectinterface_resolution {
    uint32_t width;
    uint32_t height;
    uint32_t color_depth;
};

typedef void (*bga_get_resolution_function)(struct object* obj, struct objectinterface_resolution* resolution);
typedef void (*bga_set_resolution_function)(struct object* obj, struct objectinterface_resolution* resolution);
typedef uint32_t (*bga_get_buffersize_function)(struct object* obj);
typedef void (*bga_blt_function)(struct object* obj, uint8_t* buffer, uint32_t buffer_size);

struct objectinterface_bga {
    bga_get_resolution_function get_resolution;
    bga_set_resolution_function set_resolution;
    bga_get_buffersize_function get_buffersize;
    bga_blt_function blt;
};

#endif