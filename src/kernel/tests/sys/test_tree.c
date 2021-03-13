//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/tree/tree.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <types.h>

char G_A[] = {"A"};
char G_B[] = {"BB"};
char G_C[] = {"CCC"};
char G_D[] = {"DDDD"};
char G_E[] = {"EEEEE"};
char G_F[] = {"FFFFFF"};

void test_tree_search() {
    struct tree* t = tree_new();

    tree_insert(t, 'A', G_A);
    tree_insert(t, 'B', G_B);
    tree_insert(t, 'C', G_C);
    tree_insert(t, 'D', G_D);
    tree_insert(t, 'E', G_E);

    uint8_t* value = tree_search(t, 'A');
    ASSERT(0 == (strcmp(value, G_A)));

    tree_delete(t);
}

void test_tree_find() {
    struct tree* t = tree_new();

    tree_insert(t, 'A', G_A);
    tree_insert(t, 'B', G_B);
    tree_insert(t, 'C', G_C);
    tree_insert(t, 'D', G_D);
    tree_insert(t, 'E', G_E);

    uint64_t key = tree_find(t, &tree_string_comparator, "B");
    ASSERT(key != 0);

    tree_delete(t);
}

void test_tree() {
    kprintf("Testing Tree\n");
    test_tree_search();
    test_tree_find();
}