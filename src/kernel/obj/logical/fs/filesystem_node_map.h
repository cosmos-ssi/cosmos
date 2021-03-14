//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* a node map is used by a filesystem to keep a binary tree of filesystem_nodes by id
*/
#ifndef _FILESYSTEM_NODE_MAP_H
#define _FILESYSTEM_NODE_MAP_H

#include <types.h>

struct tree;
struct filesystem_node;

/*
* tree of nodes.  key is id, value is string
*/
struct filesystem_node_map {
    struct tree* filesystem_nodes_by_id;
};

struct filesystem_node_map* filesystem_node_map_new();
void filesystem_node_map_delete(struct filesystem_node_map* map);
void filesystem_node_map_clear(struct filesystem_node_map* map);
void filesystem_node_map_insert(struct filesystem_node_map* map, struct filesystem_node* node);
struct filesystem_node* filesystem_node_map_find_id(struct filesystem_node_map* map, uint64_t id);
uint64_t filesystem_node_map_find_name(struct filesystem_node_map* map, uint8_t* name);

void filesystem_node_map_get_node_name(struct filesystem_node_map* map, struct filesystem_node* node, uint8_t* name,
                                       uint32_t name_size);
#endif
