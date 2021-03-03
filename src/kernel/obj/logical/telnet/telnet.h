//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TELNET_H
#define _TELNET_H

struct object;

struct object* telnet_attach(struct object* serial_device);
void telnet_detach(struct object* obj);

#endif