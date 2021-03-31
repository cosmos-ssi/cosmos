//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include <types.h>

/**
 *  a simple list based on an array.  The array is automatically expanded as required to contain all the list items
 */
/**
 * an interator function is provded since for loops are notoriously the cause of bugs
 */
/**
 * indices are zero-based
 */
struct arraylist {
    struct array* arr;
    uint32_t count;
};

/*
 * list iterator
 */
typedef void (*list_iterator)(void* value);

/*
 * comparator
 */
typedef uint8_t (*list_comparator)(void* value1, void* value2);
uint8_t arraylist_string_comparator(void* e1, void* e2);

/*
 * new list
 */
struct arraylist* arraylist_new();
/*
 * delete list
 */
void arraylist_delete(struct arraylist* lst);
/*
 * number items in the list
 */
uint32_t arraylist_count(struct arraylist* lst);
/*
 * size of the underlying array
 */
uint32_t arraylist_size(struct arraylist* lst);
/*
 * returns zero-based index of added item
 */
uint32_t arraylist_add(struct arraylist* lst, void* value);
/*
 * set value at index
 */
void arraylist_set(struct arraylist* lst, uint32_t position, void* value);
/*
 * get value at index
 */
void* arraylist_get(struct arraylist* lst, uint32_t position);
/*
 * iterate
 */
void arraylist_iterate(struct arraylist* lst, list_iterator iter);
/*
 * remove element at index
 */
void arraylist_remove(struct arraylist* lst, uint32_t position);
/*
* sort
*/
void arraylist_sort(struct arraylist* lst, list_comparator comparator);

#endif