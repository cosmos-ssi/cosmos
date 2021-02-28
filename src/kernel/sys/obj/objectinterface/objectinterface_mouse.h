//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all mouse objects will implement
 */
#ifndef _OBJECTTYPE_MOUSE_H
#define _OBJECTTYPE_MOUSE_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

struct mouse_status {
    uint8_t mouse_cycle;
    int8_t mouse_byte[3];
    int8_t mouse_x;
    int8_t mouse_y;
};

typedef struct mouse_status* (*mouse_status_function)(struct object* obj);

struct objectinterface_mouse {
    mouse_status_function status;
};

#endif