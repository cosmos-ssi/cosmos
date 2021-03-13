//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTTYPE_FILESYSTEM_H
#define _OBJECTTYPE_FILESYSTEM_H

#include <types.h>

struct object;

#define FILESYSTEM_MAX_NAME 128
#define FILESYSTEM_MAX_FILES_PER_DIR 1024
#define FILESYSTEM_NAME_DELIMITER "/"
#define FILESYSTEM_MAX_DEPTH 255

enum filesystem_node_type { folder, file, object };

typedef struct filesystem_node {
    /*
    * type
    */
    enum filesystem_node_type type;
    /* 
    * owning filesystem object
    */
    struct object* filesystem_obj;
    /*
    * id
    */
    uint64_t id;
    /*
    * name
    */
    uint8_t name[FILESYSTEM_MAX_NAME];
    /*
    * node_specific data
    */
    void* node_data;
    /*
    * parent node id.  zero for root node
    */
    uint64_t parent;
} filesystem_node_t;

struct filesystem_directory {
    uint64_t count;
    uint64_t ids[FILESYSTEM_MAX_FILES_PER_DIR];
};

/*
* get root node
*/
typedef struct filesystem_node* (*filesystem_get_root_node_function)(struct object* filesystem_obj);

/*
* read bytes from node
*/
typedef uint32_t (*filesystem_read_function)(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size);
/*
* write bytes to node
*/
typedef uint32_t (*filesystem_write_function)(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size);
typedef void (*filesystem_open_function)(struct filesystem_node* fs_node);
typedef void (*filesystem_close_function)(struct filesystem_node* fs_node);
/*
* find a node by id
*/
typedef struct filesystem_node* (*filesystem_find_node_by_id_function)(struct filesystem_node* fs_node, uint64_t id);
/*
* get directory list.  fills struct. 
*/
typedef void (*filesystem_list_directory_function)(struct filesystem_node* fs_node, struct filesystem_directory* dir);
/*
* get file size 
*/
typedef uint64_t (*filesystem_size_function)(struct filesystem_node* fs_node);

struct objectinterface_filesystem {
    filesystem_get_root_node_function root;
    filesystem_read_function read;
    filesystem_write_function write;
    filesystem_open_function open;
    filesystem_close_function close;
    filesystem_find_node_by_id_function find_id;
    filesystem_list_directory_function list;
    filesystem_size_function size;
};

#endif