//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEREGISTRY_H
#define _DEVICEREGISTRY_H

#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <types.h>

void deviceregistry_init();
void deviceregistry_registerdevice(struct device* dev);
void deviceregistry_unregisterdevice(struct device* dev);

uint16_t deviceregistry_devicecount();
uint16_t deviceregistry_devicecount_type(device_type dt);
struct device* deviceregistry_get_device(device_type dt, uint16_t idx);

typedef void (*device_iterator)(struct device* dev);

void deviceregistry_iterate(device_iterator deviceIterator);
void deviceregistry_iterate_type(device_type dt, device_iterator deviceIterator);

// find a device by name ie "rtc0"
struct device* deviceregistry_findDevice(const int8_t* name);

// find devices by devicetype and description
void deviceregistry_find_devices_by_description(device_type dt, const int8_t* description, deviceSearchCallback cb);

// find devices by devicetype
void deviceregistry_find_devices_by_devicetype(device_type dt, deviceSearchCallback cb);

#endif