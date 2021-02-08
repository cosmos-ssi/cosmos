//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
    CosmOS models a fs as a recursive tree of fs's.
*/
#ifndef _DEVICEAPI_FILESYSTEM_H
#define _DEVICEAPI_FILESYSTEM_H

#include <sys/vfs/vfs_node.h>
#include <types.h>

struct device;

typedef struct vfs_node* (*fs_get_vfs_root_node)(struct device* dev);

/// TODO REMOVE THIS.  WE NEVER USE IT.
/*
* a fs has only one api current: an API to create a vfs node
*/
struct deviceapi_filesystem {
    fs_get_vfs_root_node vfs;
};

#endif