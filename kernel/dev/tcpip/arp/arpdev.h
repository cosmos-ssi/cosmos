//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARPDEV_H
#define _ARPDEV_H

#include <types.h>

struct device;

struct device* arp_attach(struct device* ethernet_device);
void arp_detach(struct device* dev);

#endif