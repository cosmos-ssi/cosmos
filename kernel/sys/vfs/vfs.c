//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/split_string.h>
#include <sys/string/string.h>
#include <sys/vfs/folder_vfs.h>
#include <sys/vfs/vfs.h>

struct vfs_node* cosmos_vfs = 0;

void vfs_init() {
    cosmos_vfs = vfs_new_folder("/");

    // local subtree
    struct vfs_node* local_folder = vfs_new_folder("localhost");
    vfs_add_child(cosmos_vfs, local_folder);

    // dev subtree
    struct vfs_node* dev_folder = vfs_new_folder("dev");
    vfs_add_child(local_folder, dev_folder);

    // fs subtree
    struct vfs_node* fs_folder = vfs_new_folder("fs");
    vfs_add_child(local_folder, fs_folder);
}

void vfs_delete(struct vfs_node* v) {
    ASSERT_NOT_NULL(v);
    if (0 != v->children) {
        uint32_t count = arraylist_count(v->children);
        for (uint32_t i = 0; i < count; i++) {
            struct vfs_node* child = (struct vfs_node*)arraylist_get(v->children, i);
            vfs_delete(child);
        }
    }
    kfree(v->name);
}

void vfs_set_name(struct vfs_node* v, uint8_t* name) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(name);
    if (0 != v->name) {
        kfree(v->name);
    }
    uint32_t size = strlen(name) + 1;
    uint8_t* s = kmalloc(size);
    strncpy(s, name, size);
    v->name = s;
}

/*
* a comparator for vfs nodes
*/
uint8_t vfs_comparator(void* e1, void* e2) {
    ASSERT_NOT_NULL(e1);
    ASSERT_NOT_NULL(e2);
    struct vfs_node* v1 = (struct vfs_node*)e1;
    struct vfs_node* v2 = (struct vfs_node*)e2;

    if (strcmp(v1->name, v2->name) == 1) {
        return 1;
    }
    return 0;
}

void vfs_add_child(struct vfs_node* v, struct vfs_node* child) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(child);
    ASSERT_NOT_NULL(child->name);
    if (0 == v->children) {
        v->children = arraylist_new();
    }
    arraylist_add(v->children, child);
    /*
    * sort on every insert.  It's a bit of overkill, but it makes finding things later much faster
    */
    arraylist_sort(v->children, &vfs_comparator);
}

void vfs_remove_child(struct vfs_node* v, uint8_t* name) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(v->children);
    ASSERT_NOT_NULL(name);
    uint32_t idx = -1;
    for (uint32_t i = 0; i < arraylist_count(v->children); i++) {
        struct vfs_node* vv = (struct vfs_node*)arraylist_get(v->children, i);
        if (0 == strcmp(vv->name, name)) {
            idx = i;
            break;
        }
    }
    if (-1 != idx) {
        arraylist_remove(v->children, idx);
    } else {
        panic("Did not find child\n");
    }
}

/*
* internal find function
*/
struct vfs_node* vfs_find_internal(struct vfs_node* v, struct arraylist* al, uint32_t depth) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(al);
    if (arraylist_count(al) == depth) {
        /*
        * last node, return it
        */
        return v;
    } else {
        if (0 != v->children) {
            /*
            * more nodes, recurse
            */
            for (uint32_t i = 0; i < arraylist_count(v->children); i++) {
                uint8_t* t = arraylist_get(al, depth);
                uint8_t* child = ((struct vfs_node*)arraylist_get(v->children, i))->name;
                if (0 == strcmp(t, child)) {
                    return vfs_find_internal(arraylist_get(v->children, i), al, depth + 1);
                }
            }
        }
        return 0;
    }
}

struct vfs_node* vfs_find(struct vfs_node* v, uint8_t* name) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(name);
    struct arraylist* al = arraylist_new();
    split_string(name, "/", al);
    struct vfs_node* ret = vfs_find_internal(v, al, 0);
    delete_string_list(al);
    return ret;
}

void vfs_traverse_internal(struct vfs_node* v, vfs_traverse_function f, uint32_t depth) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(f);
    // call the callback
    (*f)(v, depth);

    // children
    if (0 != v->children) {
        for (uint32_t i = 0; i < arraylist_count(v->children); i++) {
            struct vfs_node* c = (struct vfs_node*)arraylist_get(v->children, i);
            vfs_traverse_internal(c, f, depth + 1);
        }
    }
}

/*
* traverse
*/
void vfs_traverse(struct vfs_node* v, vfs_traverse_function f) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(f);
    vfs_traverse_internal(v, f, 0);
}

void vfs_dump_traverse_function(struct vfs_node* v, uint32_t depth) {
    ASSERT_NOT_NULL(v);
    for (int32_t i = 0; i < depth; i++) {
        kprintf(" ");
    }
    kprintf("%s\n", v->name);
}

void vfs_dump(struct vfs_node* v) {
    ASSERT_NOT_NULL(v);
    vfs_traverse(v, &vfs_dump_traverse_function);
}

uint32_t vfs_count(struct vfs_node* v) {
    ASSERT_NOT_NULL(v);
    if (0 == v->children) {
        return 0;
    } else {
        return arraylist_count(v->children);
    }
}
