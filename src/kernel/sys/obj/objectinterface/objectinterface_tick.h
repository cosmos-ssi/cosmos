//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all tick objects will implement
 */
#ifndef _OBJECTTYPE_TICK_H
#define _OBJECTTYPE_TICK_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef uint64_t (*tick_read_function)(struct object* obj);

struct objectinterface_tick {
    tick_read_function read;
};

#endif