
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}
#include <assert.hpp>
#include <collection/tree.hpp>

template <typename T> Tree<T>::Tree() {
    this->key = 0;
    this->value = 0;
    this->left = 0;
    this->right = 0;
}

/*
 * warning, recursive
 */
template <typename T> Tree<T>::~Tree() {
    if (0 != this->left) {
        delete (this->left);
    }
    if (0 != this->right) {
        delete (this->right);
    }
}

/*
 * warning, recursive
 */
template <typename T> void Tree<T>::insert(uint64_t key, T* value) {
    if (key < this->key) {
        if (0 != this->left) {
            this->left->insert(key, value);
        } else {
            this->left = new Tree();
            this->lefthis->key = key;
            this->lefthis->value = value;
        }
    } else {
        if (0 != this->right) {
            this->right->insert(key, value);
        } else {
            this->right = new Tree();
            this->righthis->key = key;
            this->righthis->value = value;
        }
    }
}

/*
 * warning, recursive
 */
template <typename T> T* Tree<T>::search(uint64_t key) {
    if (key == this->key) {
        return this->value;
    } else if (key < this->key) {
        if (0 != this->left) {
            return this->left->search(key);
        } else {
            return 0;
        }
    } else {
        if (0 != this->right) {
            return this->right->search(key);
        } else {
            return 0;
        }
    }
}
