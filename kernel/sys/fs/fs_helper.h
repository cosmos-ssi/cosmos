//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FS_HELPER_H
#define _FS_HELPER_H

#include <types.h>

struct filesystem_node;
struct device;

//void fshelper_remove_device_from_vfs(struct device* vfs_device, struct device* dev);
//void fshelper_add_device_to_vfs(struct device* vfs_device, struct device* dev);

struct filesystem_node* fshelper_get_fs_node(struct device* filesystem_device);

//typedef void (*vfshelper_traverse_function)(struct vfs_node* v, uint32_t depth);

//uint32_t vfs_count(struct vfs_node* v);
//void vfs_traverse(struct vfs_node* v, vfs_traverse_function f);
//void vfs_dump(struct vfs_node* v);

// the VFS for this host
//extern struct vfs_node* cosmos_vfs;

#endif