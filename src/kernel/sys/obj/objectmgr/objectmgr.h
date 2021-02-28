//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H

#include <sys/obj/object/object.h>
#include <sys/obj/objecttype/objectype.h>

#include <types.h>

// register a object
void objectmgr_register_object(struct object* obj);

// unregister a object
void objectmgr_unregister_object(struct object* obj);

// init the object registry
void objectmgr_init();

// register all the objects
void objectmgr_register_objects();

// count of object instances
uint16_t objectmgr_object_count();

// init all objects
void objectmgr_init_objects();

// set description
void objectmgr_set_object_description(struct object* obj, const uint8_t* description);

// find a object ie ("rtc0")
struct object* objectmgr_find_object(const uint8_t* name);

// find objects by the object description
typedef void (*objectSearchCallback)(struct object* obj);
void objectmgr_find_objects_by_description(enum object_type_id dt, const uint8_t* description, objectSearchCallback cb);

// find objects by object_type
void objectmgr_find_objects_by_object_type(enum object_type_id dt, objectSearchCallback cb);

// attach a object (non-fixed objects... like RAM disks and SWAP)
uint8_t objectmgr_attach_object(struct object* obj);

// detach a object (non-fixed objects... like RAM disks and SWAP)
// be aware, this has the effect of deleting the dev struct!
uint8_t objectmgr_detach_object(struct object* obj);

/*
* increment object reference count
*/
uint8_t objectmgr_increment_object_refcount(struct object* obj);

/*
* decrease object reference count
*/
uint8_t objectmgr_decrement_object_refcount(struct object* obj);

/*
* dump all objects to console
*/
void objectmgr_dump_objects();
#endif