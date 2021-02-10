//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NODE_CACHE_H
#define _NODE_CACHE_H

#include <types.h>

struct filesystem_node;

void node_cache_add(struct filesystem_node* fs_node);
struct filesystem_node* node_cache_find(uint32_t id);
void node_cache_remove(uint32_t id);

#endif