//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PIT_H
#define _PIT_H

#include <types.h>

void pit_devicemgr_register_devices();

typedef void (*PITEvent)();

void pit_subscribe(PITEvent pitEvent);
uint64_t pit_tickcount();
#endif