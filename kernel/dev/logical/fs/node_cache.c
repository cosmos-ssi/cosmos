//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/node_cache.h>
#include <sys/collection/tree/tree.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/panic/panic.h>

struct tree* node_cache_tree;

void node_cache_add(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    if (0 == node_cache_tree) {
        node_cache_tree = tree_new();
    }
    tree_insert(node_cache_tree, fs_node->id, fs_node);
}

struct filesystem_node* node_cache_find(uint32_t id) {
    if (0 == node_cache_tree) {
        return 0;
    } else {
        return tree_search(node_cache_tree, id);
    }
}

void node_cache_remove(uint32_t id) {
    panic("Not Implemented");
}
