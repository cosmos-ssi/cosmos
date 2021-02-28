//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTYPES_H
#define _OBJECTYPES_H

#include <sys/obj/objecttype/objectype.h>
#include <types.h>

void objecttypes_init();
uint32_t objecttypes_count();
struct object_type* objecttypes_find(uint16_t id);
struct object_type* objecttypes_get(uint32_t i);
void objecttypes_add(struct object_type* ot);

#endif