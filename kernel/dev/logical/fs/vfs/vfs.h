//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VFS_H
#define _VFS_H

#include <types.h>

struct filesystem_node;

struct device* vfs_attach();
void vfs_detach(struct device* dev);

void vfs_add_child(struct device* vfs_device, struct filesystem_node* child_node);
void vfs_remove_child(struct device* vfs_device, uint64_t id);

#endif