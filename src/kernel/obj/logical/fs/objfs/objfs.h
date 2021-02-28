//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJFS_H
#define _OBJFS_H

#include <types.h>

struct object* objfs_attach();
void objfs_detach(struct object* obj);

#endif