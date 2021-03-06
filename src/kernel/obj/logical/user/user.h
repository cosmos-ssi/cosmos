//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
#ifndef _USER_H
#define _USER_H

#include <types.h>

struct object;

struct object* user_attach(uint8_t* name);
void user_detach(struct object* obj);

#endif