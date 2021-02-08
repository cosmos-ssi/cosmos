//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* vfs nodes for filesystem 
*/
#ifndef _FILESYSTEMVFS_H
#define _FILESYSTEMVFS_H

#include <sys/vfs/vfs.h>

struct vfs_node* vfs_new_filesystem(uint8_t* devicename);

#endif