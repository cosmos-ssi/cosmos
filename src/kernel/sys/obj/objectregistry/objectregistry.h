//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTREGISTRY_H
#define _OBJECTREGISTRY_H

#include <sys/obj/objectmgr/objectmgr.h>
#include <types.h>

struct object;

void objectregistry_init();
void objectregistry_registerobject(struct object* obj);
void objectregistry_unregisterobject(struct object* obj);

uint32_t objectregistry_objectcount();
uint32_t objectregistry_objectcount_type(uint16_t dt);
struct object* objectregistry_get_object(uint16_t dt, uint16_t idx);

typedef void (*object_iterator)(struct object* obj);

void objectregistry_iterate(object_iterator objectIterator);
void objectregistry_iterate_type(uint16_t dt, object_iterator objectIterator);

// find a object by name ie "rtc0"
struct object* objectregistry_find_object_by_name(const int8_t* name);

// find a object by handle
struct object* objectregistry_find_object_by_handle(uint64_t handle);

// find objects by objectype and description
void objectregistry_find_objects_by_description(uint16_t dt, const int8_t* description, objectSearchCallback cb);

// find objects by objectype
void objectregistry_find_objects_by_objectype(uint16_t dt, objectSearchCallback cb);

#endif