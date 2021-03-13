
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TREE_H
#define _TREE_H

#include <types.h>

typedef void (*tree_iterator)(void* value);
typedef uint8_t (*tree_comparator)(void* value1, void* value2);
uint8_t tree_string_comparator(void* e1, void* e2);

struct tree {
    void* value;
    uint64_t key;
    struct tree* left;
    struct tree* right;
};

struct tree* tree_new();
void tree_delete(struct tree* t);
void tree_insert(struct tree* t, uint64_t key, void* value);
void* tree_search(struct tree* t, uint64_t key);
void tree_iterate(struct tree* t, tree_iterator iter);
uint64_t tree_find(struct tree* t, tree_comparator comparator, void* value);

#endif
