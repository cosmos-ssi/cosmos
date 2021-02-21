//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all BGA devices will implement
 */
#ifndef _DEVICEAPI_BGA_H
#define _DEVICEAPI_BGA_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

struct deviceapi_resolution {
    uint32_t width;
    uint32_t height;
    uint32_t color_depth;
};

typedef void (*bga_get_resolution_function)(struct device* dev, struct deviceapi_resolution* resolution);
typedef void (*bga_set_resolution_function)(struct device* dev, struct deviceapi_resolution* resolution);
typedef uint32_t (*bga_get_buffersize_function)(struct device* dev);
typedef void (*bga_blt_function)(struct device* dev, uint8_t* buffer, uint32_t buffer_size);

struct deviceapi_bga {
    bga_get_resolution_function get_resolution;
    bga_set_resolution_function set_resolution;
    bga_get_buffersize_function get_buffersize;
    bga_blt_function blt;
};

#endif