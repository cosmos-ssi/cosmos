//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all Ethernet devices will implement
*/
#ifndef _DEVICEAPI_ETHERNET_H
#define _DEVICEAPI_ETHERNET_H

#include <types.h>
#include <devicemgr/devicemgr.h>

typedef void (*ethernet_read)(struct device* dev, uint8_t* data, uint8_t* size);
typedef void (*ethernet__write)(struct device* dev, uint8_t* data, uint8_t* size);

struct deviceapi_ethernet {
    ethernet_read read;
    ethernet__write write;
};

#endif