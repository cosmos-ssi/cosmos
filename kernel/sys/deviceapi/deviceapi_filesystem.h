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

/*
* read bytes
*/
typedef uint32_t (*filesystem_read_function)(struct device* filesystem_device, const uint8_t* data, uint32_t data_size);
/*
* write bytes
*/
typedef uint32_t (*filesystem_write_function)(struct device* filesystem_device, const uint8_t* data,
                                              uint32_t data_size);
typedef void (*filesystem_open_function)(struct device* filesystem_device);
typedef void (*filesystem_close_function)(struct device* filesystem_device);
/*
* find a node by id
*/
typedef struct filesystem_node* (*filesystem_find_node_by_id_function)(struct device* filesystem_device, uint32_t idx);
/*
* find a node by name
*/
typedef struct filesystem_node* (*filesystem_find_node_by_name_function)(struct device* filesystem_device,
                                                                         uint8_t* name);

struct deviceapi_filesystem {
    filesystem_read_function read;
    filesystem_write_function write;
    filesystem_open_function open;
    filesystem_close_function close;
    filesystem_find_node_by_id_function find_id;
    filesystem_find_node_by_name_function find_name;
};

#endif