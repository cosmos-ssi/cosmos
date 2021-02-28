//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all swap devices will implement
 */
#ifndef _OBJECTTYPE_SWAP_H
#define _OBJECTTYPE_SWAP_H

#include <sys/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*swap_read_function)(struct object* dev, uint8_t* data, uint32_t block);
typedef void (*swap_write_function)(struct object* dev, uint8_t* data, uint32_t block);
typedef uint16_t (*swap_block_size_function)(struct object* dev);
typedef uint16_t (*swap_block_count_function)(struct object* dev);

struct objecttype_swap {
    swap_read_function read;
    swap_write_function write;
    swap_block_size_function block_size;
    swap_block_count_function block_count;
};

#endif