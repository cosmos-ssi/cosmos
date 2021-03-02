//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * kernel telnet
 */
#ifndef _OBJECTTYPE_TELNET_H
#define _OBJECTTYPE_TELNET_H

#include <types.h>

typedef void (*telnet_start_function)(struct object* obj);

struct objectinterface_telnet {
    telnet_start_function start;
};

#endif