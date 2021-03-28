//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}

#include <array.hpp>
#include <assert.hpp>

template <typename T> Array<T>::Array(uint32_t size) {
    this->byte_size = sizeof(T*) * size;
    this->data = (T**)malloc(this->byte_size);
    this->size = size;
    for (uint32_t i = 0; i < size; i++) {
        this->data[i] = 0;
    }
}

template <typename T> Array<T>::~Array() {
    if (0 != this->data) {
        free(this->data);
    }
}

template <typename T> uint32_t Array<T>::getSize() {
    return this->size;
}

template <typename T> void Array<T>::set(uint32_t position, T* value) {
    if ((position >= 0) && (position < this->size)) {
        this->data[position] = value;
    }
}

template <typename T> T* Array<T>::get(uint32_t position) {
    if ((position >= 0) && (position < this->size)) {
        return this->data[position];
    }
    return 0;
}

template <typename T> void Array<T>::resize(uint32_t size) {
    if (size >= this->size) {
        /*
         * TODO.  Should use krealloc here, but I think there is a bug in krealloc
         */
        uint32_t oldsize = this->size;
        void** oldata = this->data;
        this->byte_size = sizeof(void*) * size;
        void** newdata = (void**)malloc(this->byte_size);
        for (uint32_t i = 0; i < oldsize; i++) {
            newdata[i] = oldata[i];
        }
        free(oldata);
        this->data = newdata;
        this->size = size;
    }
}

template <typename T> void Array<T>::grow(uint32_t increment) {
    resize(increment + (this->size));
}
