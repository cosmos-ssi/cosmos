//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/filesystem_node_map.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/collection/tree/tree.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/string/string.h>

struct filesystem_node_map* filesystem_node_map_new() {
    struct filesystem_node_map* ret = (struct filesystem_node_map*)kmalloc(sizeof(struct filesystem_node_map));
    ret->filesystem_nodes_by_id = tree_new();
    return ret;
}

void filesystem_node_map_delete(struct filesystem_node_map* map) {
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(map->filesystem_nodes_by_id);
    tree_delete(map->filesystem_nodes_by_id);
    kfree(map);
}

void filesystem_delete_tree_iterator(void* value) {
    if (0 != value) {
        kfree(value);
    }
}

void filesystem_node_map_clear(struct filesystem_node_map* map) {
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(map->filesystem_nodes_by_id);
    tree_iterate(map->filesystem_nodes_by_id, &filesystem_delete_tree_iterator);
}

void filesystem_node_map_insert(struct filesystem_node_map* map, struct filesystem_node* node) {
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(map->filesystem_nodes_by_id);
    ASSERT_NOT_NULL(node);
    ASSERT_NOT_NULL(node->id);
    tree_insert(map->filesystem_nodes_by_id, node->id, node);
}

struct filesystem_node* filesystem_node_map_find_id(struct filesystem_node_map* map, uint64_t id) {
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(map->filesystem_nodes_by_id);
    return tree_search(map->filesystem_nodes_by_id, id);
}

uint8_t filesystem_node_map_name_comparator(void* criteria, void* value) {
    ASSERT_NOT_NULL(criteria);
    ASSERT_NOT_NULL(value);
    uint8_t* name = (uint8_t*)criteria;
    struct filesystem_node* node = (struct filesystem_node*)value;
    //   kprintf("   name: %s\n", name);
    //  kprintf("   node name: %s\n", node->name);
    //  kprintf("   node id: %llu\n", node->id);

    if (0 == strcmp(name, node->name)) {
        return 1;
    }
    return 0;
}

uint64_t filesystem_node_map_find_name(struct filesystem_node_map* map, uint8_t* name) {
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(map->filesystem_nodes_by_id);
    ASSERT_NOT_NULL(name);
    return tree_find(map->filesystem_nodes_by_id, &filesystem_node_map_name_comparator, name);
}

void filesystem_node_map_get_node_name(struct filesystem_node_map* map, uint64_t filesystem_node_id,
                                       const uint8_t* name, uint32_t name_size) {
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(map->filesystem_nodes_by_id);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(name_size);

    struct filesystem_node* thisNode = filesystem_node_map_find_id(map, filesystem_node_id);
    ASSERT_NOT_NULL(thisNode);
    struct arraylist* lst = arraylist_new();

    // make a list
    while (0 != thisNode) {
        arraylist_add(lst, (void*)thisNode->id);
        uint64_t parentId = thisNode->parent;
        if (0 != parentId) {
            thisNode = filesystem_node_map_find_id(map, parentId);
        } else {
            thisNode = 0;
        }
    }
    // walk the list back down, making a name
    for (uint64_t i = 0; i < arraylist_count(lst); i++) {
        uint64_t id = (uint64_t)arraylist_get(lst, i);
        kprintf("id %llu\n", id);
    }
}
