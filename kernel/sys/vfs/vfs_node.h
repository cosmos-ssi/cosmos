//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VFS_NODE_H
#define _VFS_NODE_H

#include <sys/collection/arraylist/arraylist.h>
#include <sys/devicemgr/devicemgr.h>
#include <types.h>

enum vfs_type { file = 0x01, folder = 0x02, device = 0x03 };

struct vfs_node;

/*
* read bytes
*/
typedef uint32_t (*vfs_read_function)(struct vfs_node* vfs, const uint8_t* data, uint32_t data_size);
/*
* write bytes
*/
typedef uint32_t (*vfs_write_function)(struct vfs_node* vfs, const uint8_t* data, uint32_t data_size);
typedef void (*vfs_open_function)(struct vfs_node* vfs);
typedef void (*vfs_close_function)(struct vfs_node* vfs);
/*
* find a node by id
*/
typedef struct vfs_node* (*vfs_find_node_by_id_function)(struct vfs_node* vfs, uint32_t idx);
/*
* find a node by name
*/
typedef struct vfs_node* (*vfs_find_node_by_name_function)(struct vfs_node* vfs, uint8_t* name);

struct vfs_node {

    enum vfs_type type;
    struct arraylist* children;

    /*
    This is a filename or a folder name
    For a device, or a fs, it's the dev name, since a fs is a dev
    */
    uint8_t* name;

    vfs_read_function read;
    vfs_write_function write;
    vfs_open_function open;
    vfs_close_function close;
    vfs_find_node_by_id_function find_id;
    vfs_find_node_by_name_function find_name;
};

#endif