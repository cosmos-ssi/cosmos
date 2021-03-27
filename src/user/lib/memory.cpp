//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/abi.h>
#include <assert.h>
#include <memory.hpp>

void Memory::free(void* ptr) {
    ASSERT_NOT_NULL(ptr);
    syscall_memory_free(ptr);
}
void* Memory::malloc(uint64_t size) {
    ASSERT_NOT_NULL(size);
    return (void*)syscall_memory_malloc(size);
}

void* Memory::realloc(void* ptr, uint64_t size) {
    ASSERT_NOT_NULL(ptr);
    ASSERT_NOT_NULL(size);
    return (void*)syscall_memory_realloc(ptr, size);
}
