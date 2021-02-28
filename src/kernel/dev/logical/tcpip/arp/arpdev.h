//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARPDEV_H
#define _ARPDEV_H

#include <sys/objecttype/objecttype_arp.h>
#include <types.h>

struct object;

struct object* arp_attach(struct object* ethernet_device);
void arp_detach(struct object* dev);

void arp_packet_init(struct arp* a, uint16_t opcode);

#endif