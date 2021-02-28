//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/node_cache.h>
#include <sys/collection/tree/tree.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objectinterface/objectinterface_filesystem.h>
#include <sys/panic/panic.h>

struct node_cache* node_cache_new() {
    struct node_cache* ret = kmalloc(sizeof(struct node_cache));
    ret->node_tree = tree_new();
    return ret;
}

void node_cache_delete(struct node_cache* nc) {
    ASSERT_NOT_NULL(nc);
    kfree(nc);
}

void node_cache_add(struct node_cache* nc, struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(nc);
    ASSERT_NOT_NULL(nc->node_tree);
    ASSERT_NOT_NULL(fs_node);
    tree_insert(nc->node_tree, fs_node->id, fs_node);
}

struct filesystem_node* node_cache_find(struct node_cache* nc, uint64_t id) {
    ASSERT_NOT_NULL(nc);
    ASSERT_NOT_NULL(nc->node_tree);
    return tree_search(nc->node_tree, id);
}

void node_cache_remove(struct node_cache* nc, uint64_t id) {
    ASSERT_NOT_NULL(nc);
    ASSERT_NOT_NULL(nc->node_tree);

    PANIC("Not Implemented");
}
