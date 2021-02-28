//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTYPES_H
#define _OBJECTTYPES_H

#include <sys/objectmgr/object.h>
#include <types.h>

struct arraylist;

#define MAX_DEVICE_TYPES 256

void objecttypes_init();
struct arraylist* objecttypes_get_objectlist(object_type dt);
void objecttypes_set_objectlist(object_type dt, struct arraylist* lst);
uint32_t objecttypes_count();
#endif