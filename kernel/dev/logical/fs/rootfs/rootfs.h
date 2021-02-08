//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ROOTFS_H
#define _ROOTFS_H

#include <types.h>

struct device* rootfs_attach();
void rootfs_detach(struct device* dev);

#endif