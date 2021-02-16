//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* fsfacade wraps a device that implements deviceapi_filesystem
*/
#ifndef _FS_FACADE_H
#define _FS_FACADE_H

#include <types.h>

struct filesystem_node;
struct filesystem_directory;
struct device;

struct filesystem_node* fsfacade_get_fs_rootnode(struct device* filesystem_device);

typedef void (*fsfacade_traverse_function)(struct filesystem_node* fs_node, uint32_t depth);

void fsfacade_traverse(struct filesystem_node* fs_node, fsfacade_traverse_function f);
void fsfacade_dump(struct filesystem_node* fs_node);

void fsfacade_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir);

struct filesystem_node* fsfacade_find_node_by_id(struct filesystem_node* fs_node, uint64_t id);

uint64_t fsfacade_size(struct filesystem_node* fs_node);

struct filesystem_node* fsfacade_find_node_by_name(struct filesystem_node* fs_node, char* name);

uint32_t fsfacade_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size);

uint32_t fsfacade_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size);
#endif