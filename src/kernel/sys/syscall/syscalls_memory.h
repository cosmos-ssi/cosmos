//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_MEMORY_H
#define _SYSCALLS_MEMORY_H

#include <types.h>

uint64_t syscall_memory_malloc(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3);
uint64_t syscall_memory_free(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3);
uint64_t syscall_memory_realloc(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3);

#endif