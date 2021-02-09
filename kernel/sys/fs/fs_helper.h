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

struct filesystem_node* fshelper_get_fs_node(struct device* filesystem_device);

typedef void (*fshelper_traverse_function)(struct filesystem_node* fs_node, uint32_t depth);

void fshelper_traverse(struct filesystem_node* fs_node, fshelper_traverse_function f);
void fshelper_dump(struct filesystem_node* fs_node);

void fshelper_count(struct filesystem_node* fs_node);

#endif