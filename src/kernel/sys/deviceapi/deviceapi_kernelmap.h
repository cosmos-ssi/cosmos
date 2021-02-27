//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all Kernelmap devices will implement
 */
#ifndef _DEVICEAPI_KERNELMAP_H
#define _DEVICEAPI_KERNELMAP_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

struct kernelmap {
    uint64_t text_start;
    uint64_t text_end;
    uint64_t data_start;
    uint64_t data_end;
    uint64_t bss_start;
    uint64_t bss_end;
    uint64_t debug_start;
    uint64_t debug_end;
};

typedef void (*kernelmap_read_function)(struct device* dev, struct kernelmap* km);

struct deviceapi_kernelmap {
    kernelmap_read_function read;
};

#endif