//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTREGISTRY_H
#define _OBJECTREGISTRY_H

#include <sys/objectmgr/objectmgr.h>

void objectregistry_init();
void objectregistry_registerdevice(struct object* obj);
void objectregistry_unregisterdevice(struct object* obj);

uint16_t objectregistry_devicecount();
uint16_t objectregistry_devicecount_type(device_type dt);
struct object* objectregistry_get_device(device_type dt, uint16_t idx);

typedef void (*device_iterator)(struct object* obj);

void objectregistry_iterate(device_iterator deviceIterator);
void objectregistry_iterate_type(device_type dt, device_iterator deviceIterator);

// find a device by name ie "rtc0"
struct object* objectregistry_find_device(const int8_t* name);

// find devices by devicetype and description
void objectregistry_find_devices_by_description(device_type dt, const int8_t* description, deviceSearchCallback cb);

// find devices by devicetype
void objectregistry_find_devices_by_devicetype(device_type dt, deviceSearchCallback cb);

#endif