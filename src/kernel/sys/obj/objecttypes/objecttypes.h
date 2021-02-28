//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTYPES_H
#define _OBJECTYPES_H

#include <sys/obj/objecttype/objectype.h>

void objecttypes_init();
uint32_t objecttypes_count();
struct object_type* objecttypes_find(enum object_type_id id);
struct object_type* objecttypes_get(uint32_t i);

#endif