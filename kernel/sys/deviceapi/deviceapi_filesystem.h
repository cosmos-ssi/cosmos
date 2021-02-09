//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEAPI_FILESYSTEM_H
#define _DEVICEAPI_FILESYSTEM_H

#include <types.h>

struct device;

#define FILESYSTEM_MAX_NAME 128

struct filesystem_node {
    /* 
    * owning device
    */
    struct device* filesystem_device;
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
};

/*
* read bytes
*/
typedef struct filesystem_node* (*filesystem_get_root_node_function)(struct device* filesystem_device);

/*
* read bytes from node
*/
typedef uint32_t (*filesystem_read_function)(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size);
/*
* write bytes to node
*/
typedef uint32_t (*filesystem_write_function)(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size);
typedef void (*filesystem_open_function)(struct filesystem_node* fs_node);
typedef void (*filesystem_close_function)(struct filesystem_node* fs_node);
/*
* find a node by id
*/
typedef struct filesystem_node* (*filesystem_find_node_by_id_function)(struct filesystem_node* fs_node, uint32_t id);
/*
* find a node by idx
*/
typedef struct filesystem_node* (*filesystem_find_node_by_idx_function)(struct filesystem_node* fs_node, uint32_t idx);
/*
* find a node by name
*/
typedef struct filesystem_node* (*filesystem_find_node_by_name_function)(struct filesystem_node* fs_node,
                                                                         uint8_t* name);
/*
* count
*/
typedef struct filesystem_node* (*filesystem_count_function)(struct filesystem_node* fs_node);

struct deviceapi_filesystem {
    filesystem_get_root_node_function root;
    filesystem_read_function read;
    filesystem_write_function write;
    filesystem_open_function open;
    filesystem_close_function close;
    filesystem_find_node_by_id_function find_id;
    filesystem_find_node_by_name_function find_name;
    filesystem_find_node_by_idx_function find_idx;
    filesystem_count_function count;
};

#endif