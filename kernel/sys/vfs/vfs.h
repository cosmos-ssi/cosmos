//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VFS_H
#define _VFS_H

#include <sys/vfs/vfs_node.h>
#include <types.h>

#define VFS_DEV_TREE "/localhost/dev"
#define VFS_FS_TREE "/localhost/fs"

typedef void (*vfs_traverse_function)(struct vfs_node* v, uint32_t depth);

/*
* interface functions for all VFS nodes 
*/
void vfs_init();
void vfs_delete(struct vfs_node* v);
void vfs_set_name(struct vfs_node* v, uint8_t* name);
void vfs_add_child(struct vfs_node* v, struct vfs_node* child);
void vfs_remove_child(struct vfs_node* v, uint8_t* name);
uint32_t vfs_count(struct vfs_node* v);
struct vfs_node* vfs_find(struct vfs_node* v, uint8_t* name);
void vfs_traverse(struct vfs_node* v, vfs_traverse_function f);
void vfs_dump(struct vfs_node* v);

// the VFS for this host
extern struct vfs_node* cosmos_vfs;

#endif