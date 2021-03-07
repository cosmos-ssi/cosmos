//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all hostid objects will implement
 */
#ifndef _OBJECTTYPE_HOSTID_H
#define _OBJECTTYPE_HOSTID_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef uint64_t (*hostid_read_function)(struct object* obj);

struct objectinterface_hostid {
    hostid_read_function read;
};

#endif