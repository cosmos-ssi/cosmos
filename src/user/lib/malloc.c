//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/abi.h>
#include <malloc.h>

void* malloc(uint64_t size) {
    return (void*)syscall_memory_malloc(size);
}

void free(void* ptr) {
    syscall_memory_free(ptr);
}
