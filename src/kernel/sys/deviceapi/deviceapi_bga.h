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

typedef void (*bga_clear_function)(struct device* dev);

struct deviceapi_bga {
    bga_clear_function clear;
};

#endif