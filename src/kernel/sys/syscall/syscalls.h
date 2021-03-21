//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <types.h>

typedef enum syscalls {
    SYSCALL_PROCESS_EXIT = 1101,
    SYSCALL_PROCESS_SLEEP = 1100,
    SYSCALL_CONSOLE_WRITE = 1401,
    SYSCALL_MEMORY_MALLOC = 2400,
    SYSCALL_MEMORY_FREE = 2401,
    SYSCALL_MEMORY_REALLOC = 2402,
    SYSCALL_MAX
} syscalls;
typedef uint64_t (*syscall_handler)(uint64_t syscall_num, void* args);

uint64_t invalid_syscall(uint64_t syscall_id, void* args);
// process
uint64_t syscall_process_exit(uint64_t syscall_id, void* args);
uint64_t syscall_process_sleep(uint64_t syscall_id, void* args);
// console
uint64_t syscall_console_write(uint64_t syscall_id, void* args);
// memory
uint64_t syscall_memory_malloc(uint64_t syscall_id, void* args);
uint64_t syscall_memory_free(uint64_t syscall_id, void* args);
uint64_t syscall_memory_realloc(uint64_t syscall_id, void* args);

#endif