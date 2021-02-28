//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all PIT devices will implement
 */
#ifndef _OBJECTTYPE_PIT_H
#define _OBJECTTYPE_PIT_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*pit_event)();
typedef uint64_t (*pit_tickcount_function)(struct object* obj);
typedef void (*pit_subscribe_function)(pit_event event);

struct objectinterface_pit {
    pit_tickcount_function tickcount;
    pit_subscribe_function subscribe;
};

#endif