//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all parallel port objects will implement
 */
#ifndef _OBJECTTYPE_PARALLEL_H
#define _OBJECTTYPE_PARALLEL_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*parallel_write_function)(struct object* obj, uint8_t* data, uint16_t size);

struct objectinterface_parallel {
    parallel_write_function write;
};

#endif