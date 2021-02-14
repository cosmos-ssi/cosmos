//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICETYPES_H
#define _DEVICETYPES_H

#include <sys/devicemgr/device.h>
#include <types.h>

struct arraylist;

#define MAX_DEVICE_TYPES 256

void devicetypes_init();
struct arraylist* devicetypes_get_devicelist(device_type dt);
void devicetypes_set_devicelist(device_type dt, struct arraylist* lst);
uint32_t devicetypes_count();
#endif