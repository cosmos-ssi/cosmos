//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVFS_H
#define _DEVFS_H

#include <types.h>

struct object* objfs_attach();
void devfs_detach(struct object* obj);

#endif