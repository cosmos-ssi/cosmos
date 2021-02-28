//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIAL_CONSOLE_H
#define _SERIAL_CONSOLE_H

struct object;

struct object* serial_console_attach(struct object* serial_device);
void serial_console_detach(struct object* obj);

#endif
