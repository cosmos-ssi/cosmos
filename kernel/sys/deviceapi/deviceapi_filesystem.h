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

#include <sys/collection/arraylist/arraylist.h>
#include <sys/devicemgr/devicemgr.h>
#include <types.h>

struct deviceapi_filesystem;

struct fs_dir_entry {
    uint8_t name[128];  // filename
    uint32_t ino;       // inode number
};

typedef uint32_t (*fs_read_function)(struct deviceapi_filesystem* fs, const uint8_t* data, uint32_t data_size);
typedef uint32_t (*fs_write_function)(struct deviceapi_filesystem* fs, const uint8_t* data, uint32_t data_size);
typedef void (*fs_open_function)(struct deviceapi_filesystem* fs);
typedef void (*fs_close_function)(struct deviceapi_filesystem* fs);
typedef struct fs_dir_entry* (*fs_readdir_function)(struct deviceapi_filesystem* fs, uint32_t idx);
typedef struct deviceapi_filesystem* (*fs_finddir_function)(struct deviceapi_filesystem* fs, uint8_t* name);

struct deviceapi_filesystem {
    fs_read_function read;
    fs_write_function write;
    fs_open_function open;
    fs_close_function close;
    fs_readdir_function readdir;
    fs_finddir_function finddir;
    uint64_t node_id;
};

#endif