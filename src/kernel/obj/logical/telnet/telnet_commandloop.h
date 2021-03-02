//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TELNET_COMMANDLOOP_H
#define _TELNET_COMMANDLOOP_H

#include <types.h>

struct object;

uint8_t telnet_command_loop(struct object* telnet_object);

#endif