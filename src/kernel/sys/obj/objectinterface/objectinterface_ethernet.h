//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all Ethernet objects will implement
 */
#ifndef _OBJECTTYPE_ETHERNET_H
#define _OBJECTTYPE_ETHERNET_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

#define ETHERNET_HW_LEN 6

struct eth_hdr {
    uint8_t dest_hw[ETHERNET_HW_LEN];
    uint8_t source_hw[ETHERNET_HW_LEN];
    uint16_t type;
    uint8_t* data;
};

typedef void (*ethernet_read_function)(struct object* obj, struct eth_hdr* eth, uint16_t size);
typedef void (*ethernet_write_function)(struct object* obj, struct eth_hdr* eth, uint16_t size);

struct objectinterface_ethernet {
    ethernet_read_function read;
    ethernet_write_function write;
};

#endif