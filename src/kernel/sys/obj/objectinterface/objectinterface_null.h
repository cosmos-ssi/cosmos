//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all null devices will implement
 */
#ifndef _OBJECTTYPE_NULL_H
#define _OBJECTTYPE_NULL_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef uint8_t (*null_read_function)(struct object* obj);

struct objectinterface_null {
    null_read_function read;
};

#endif