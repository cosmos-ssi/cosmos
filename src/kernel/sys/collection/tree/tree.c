
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/tree/tree.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>

struct tree* tree_new() {
    struct tree* ret = (struct tree*)kmalloc(sizeof(struct tree));
    ret->key = 0;
    ret->value = 0;
    ret->left = 0;
    ret->right = 0;
    return ret;
}

/*
 * warning, recursive
 */
void tree_delete(struct tree* t) {
    ASSERT_NOT_NULL(t);
    if (0 != t->left) {
        tree_delete(t->left);
    }
    if (0 != t->right) {
        tree_delete(t->right);
    }
    kfree(t);
}

/*
 * warning, recursive
 */
void tree_insert(struct tree* t, uint64_t key, void* value) {
    ASSERT_NOT_NULL(t);
    if (key < t->key) {
        if (0 != t->left) {
            tree_insert(t->left, key, value);
        } else {
            t->left = tree_new();
            t->left->key = key;
            t->left->value = value;
        }
    } else {
        if (0 != t->right) {
            tree_insert(t->right, key, value);
        } else {
            t->right = tree_new();
            t->right->key = key;
            t->right->value = value;
        }
    }
}

/*
 * warning, recursive
 */
void* tree_search(struct tree* t, uint64_t key) {
    ASSERT_NOT_NULL(t);
    if (key == t->key) {
        return t->value;
    } else if (key < t->key) {
        if (0 != t->left) {
            return tree_search(t->left, key);
        } else {
            return 0;
        }
    } else {
        if (0 != t->right) {
            return tree_search(t->right, key);
        } else {
            return 0;
        }
    }
}

/*
 * warning, recursive
 */
void tree_iterate(struct tree* t, tree_iterator iter) {
    ASSERT_NOT_NULL(t);
    ASSERT_NOT_NULL(iter);

    (*iter)(t->value);
    if (0 != t->left) {
        tree_iterate(t->left, iter);
    }
    if (0 != t->right) {
        tree_iterate(t->right, iter);
    }
}

/*
* a comparator for strings
*/
uint8_t tree_string_comparator(void* e1, void* e2) {
    ASSERT_NOT_NULL(e1);
    ASSERT_NOT_NULL(e2);
    if (strcmp((uint8_t*)e1, (uint8_t*)e2) == 1) {
        return 1;
    }
    return 0;
}

/*
 * warning, recursive and O(n)
 */
uint64_t tree_find(struct tree* t, tree_comparator comparator, void* value) {
    ASSERT_NOT_NULL(t);
    ASSERT_NOT_NULL(comparator);
    ASSERT_NOT_NULL(value);

    if (0 != t->value) {
        if (1 == (*comparator)(value, t->value)) {
            return t->key;
        }
    }
    if (0 != t->left) {
        return tree_find(t->left, comparator, value);
    }
    if (0 != t->right) {
        return tree_find(t->right, comparator, value);
    }
    return 0;
}
