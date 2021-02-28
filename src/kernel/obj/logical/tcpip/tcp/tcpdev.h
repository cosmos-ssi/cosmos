//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TCPDEV_H
#define _TCPDEV_H

#include <types.h>

struct object;

struct object* tcp_attach(struct object* ip_device);
void tcp_detach(struct object* dev);

#endif