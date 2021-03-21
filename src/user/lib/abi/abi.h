//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* cosmos syscalls
*/

#ifndef _ABI_H
#define _ABI_H

#include <types.h>

// process
uint64_t syscall_process_exit();
uint64_t syscall_process_sleep(uint64_t time);

// console
uint64_t syscall_console_write(uint8_t* str);

// memory
uint64_t syscall_memory_malloc(uint64_t size);
uint64_t syscall_memory_free(void* mem);
uint64_t syscall_memory_realloc(void* mem, uint64_t size);
#endif