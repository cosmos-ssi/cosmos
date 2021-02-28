//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all Serial devices will implement
 */
#ifndef _OBJECTTYPE_SERIAL_H
#define _OBJECTTYPE_SERIAL_H

#include <sys/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*serial_write_function)(struct object* obj, const int8_t* c);

struct objecttype_serial {
    serial_write_function write;
};

#endif