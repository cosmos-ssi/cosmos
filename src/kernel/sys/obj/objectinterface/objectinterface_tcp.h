//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all TCP objects will implement
 */
#ifndef _OBJECTTYPE_TCP_H
#define _OBJECTTYPE_TCP_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*tcp_read_function)(struct object* obj, uint8_t* data, uint16_t size);
typedef void (*tcp_write_function)(struct object* obj, uint8_t* data, uint16_t size);

struct objectinterface_tcp {
    tcp_read_function read;
    tcp_write_function write;
};

#endif