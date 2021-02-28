//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _IPDEV_H
#define _IPDEV_H

#include <sys/objecttype/objecttype_ip.h>
#include <types.h>

struct object;

struct object* ip_attach(struct object* ethernet_device);
void ip_detach(struct object* obj);

uint16_t ip_checksum(uint16_t* addr, int count);

void ip_header_init(struct ip_header* header, uint16_t total_length, uint16_t protocol, uint32_t source, uint32_t dest);

#endif