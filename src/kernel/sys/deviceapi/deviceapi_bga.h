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

typedef void (*bga_clear_function)(struct device* dev, uint32_t color);
typedef uint32_t (*bga_get_width_function)(struct device* dev);
typedef uint32_t (*bga_get_height_function)(struct device* dev);
typedef uint32_t (*bga_get_colordepth_function)(struct device* dev);
typedef uint32_t (*bga_get_buffersize_function)(struct device* dev);
typedef void (*bga_blt_function)(struct device* dev, uint8_t* buffer, uint32_t buffer_size);

struct deviceapi_bga {
    bga_clear_function clear;
    bga_get_width_function get_width;
    bga_get_height_function get_height;
    bga_get_colordepth_function get_colordepth;
    bga_get_buffersize_function get_buffersize;
    bga_blt_function blt;
};

#endif