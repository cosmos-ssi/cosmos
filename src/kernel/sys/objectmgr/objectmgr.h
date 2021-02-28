//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H

#include <sys/objectmgr/object.h>
#include <types.h>

// new device, allocated on the kernel heap
struct object* objectmgr_new_device();

// register a device
void objectmgr_register_device(struct object* dev);

// unregister a device
void objectmgr_unregister_device(struct object* dev);

// init the device registry
void objectmgr_init();

// register all the devices
void objectmgr_register_devices();

// count of device instances
uint16_t objectmgr_device_count();

// init all devices
void objectmgr_init_devices();

// set description
void objectmgr_set_device_description(struct object* dev, const uint8_t* description);

// find a device ie ("rtc0")
struct object* objectmgr_find_device(const uint8_t* name);

// find devices by the device description
typedef void (*deviceSearchCallback)(struct object* dev);
void objectmgr_find_devices_by_description(device_type dt, const uint8_t* description, deviceSearchCallback cb);

// find devices by device_type
void objectmgr_find_devices_by_device_type(device_type dt, deviceSearchCallback cb);

// attach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_attach_device(struct object* dev);

// detach a device (non-fixed devices... like RAM disks and SWAP)
// be aware, this has the effect of deleting the dev struct!
uint8_t objectmgr_detach_device(struct object* dev);

/*
* increment device reference count
*/
uint8_t objectmgr_increment_device_refcount(struct object* dev);

/*
* decrease device reference count
*/
uint8_t objectmgr_decrement_device_refcount(struct object* dev);

/*
* dump all devices to console
*/
void objectmgr_dump_devices();
#endif