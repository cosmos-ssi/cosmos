//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ICMPDEV_H
#define _ICMPDEV_H

#include <types.h>

struct object;

struct object* icmp_attach(struct object* ethernet_device);
void icmp_detach(struct object* dev);

#endif