//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/malloc.h>

void* malloc(uint64_t size) {
    return 0;
}

void free(void* ptr) {}

//void* operator new(uint64_t count) {
//    return malloc(count);
//}
//void operator delete(void* ptr) {
//    return free(ptr);
//}
