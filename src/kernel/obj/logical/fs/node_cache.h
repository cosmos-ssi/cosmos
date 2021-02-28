//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* node caches are used by deviceapi_filesystem devices
*/
#ifndef _NODE_CACHE_H
#define _NODE_CACHE_H

#include <types.h>

struct filesystem_node;

struct node_cache {
    struct tree* node_tree;
};

struct node_cache* node_cache_new();
void node_cache_delete(struct node_cache* nc);
void node_cache_add(struct node_cache* nc, struct filesystem_node* fs_node);
struct filesystem_node* node_cache_find(struct node_cache* nc, uint64_t id);
void node_cache_remove(struct node_cache* nc, uint64_t id);

#endif