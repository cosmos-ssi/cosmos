//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}

#include <arraylist.hpp>
#include <assert.hpp>

#define EXPAND_SIZE 64
#define START_SIZE 16

template <typename T> ArrayList<T>::ArrayList() {
    this->arr = new Array<T>(START_SIZE);
    this->count = 0;
}

template <typename T> ArrayList<T>::~ArrayList() {
    delete this->arr;
}

template <typename T> uint32_t ArrayList<T>::getCount() {
    return this->count;
}

template <typename T> uint32_t ArrayList<T>::getSize() {
    return this->arr->size;
}

template <typename T> uint32_t ArrayList<T>::add(T* value) {
    // expand the underlying array?
    if (this->count + 1 == array_size(this->arr)) {
        array_grow(this->arr, EXPAND_SIZE);
    }
    // save the data
    array_set(this->arr, this->count, value);
    this->count = this->count + 1;

    // return the index for later use by set, get, etc
    return this->count - 1;
}

template <typename T> void ArrayList<T>::set(uint32_t position, T* value) {
    if ((position >= 0) && (position < this->count)) {
        this->arr->data[position] = value;
    }
}

template <typename T> T* ArrayList<T>::get(uint32_t position) {
    if ((position >= 0) && (position < this->count)) {
        return this->arr->data[position];
    }
    return 0;
}

template <typename T> void ArrayList<T>::remove(uint32_t position) {
    if ((position >= 0) && (position < this->count)) {
        if (position != this->count - 1) {
            for (uint32_t i = position; i < this->count - 1; i++) {
                this->arr->data[i] = this->arr->data[i + 1];
            }
        }
        this->count = this->count - 1;
    }
}
