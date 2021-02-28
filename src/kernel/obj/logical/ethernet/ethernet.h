//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <sys/objecttype/objecttype_ethernet.h>
#include <types.h>
struct object;

struct object* ethernet_attach(struct object* nic_device);
void ethernet_detach(struct object* dev);

// hw_source and hw_dest are 6 bytes
void ethernet_init_eth(struct eth_hdr* eth, uint8_t* hw_source, uint8_t* hw_dest, uint8_t* data, uint16_t payload_size);
#endif