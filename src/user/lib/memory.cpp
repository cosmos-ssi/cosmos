//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <memory.hpp>

void Memory::free(void* ptr) {}
void* Memory::malloc(uint64_t size) {
    // syscall
    return 0;
}

void* Memory::realloc(void* ptr, uint64_t size) {
    // syscall
    return 0;
}
