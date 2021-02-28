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

struct arraylist;

#define MAX_OBJECT_TYPES 256

void objecttypes_init();
struct arraylist* objecttypes_get_objectlist(enum object_type_id dt);
void objecttypes_set_objectlist(enum object_type_id dt, struct arraylist* lst);
uint32_t objecttypes_count();
#endif