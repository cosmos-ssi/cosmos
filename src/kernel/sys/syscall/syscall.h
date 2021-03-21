/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

// syscall numbers are defined in userland.md, syscall.h (userland) and syscall.h (kernel)
typedef enum syscalls {
    // process
    SYSCALL_PROCESS_EXIT = 1101,
    SYSCALL_PROCESS_SLEEP = 1100,
    // console
    SYSCALL_CONSOLE_WRITE = 1401,
    // serial
    SYSCALL_SERIAL_READCHAR = 2300,
    SYSCALL_SERIAL_WRITECHAR = 2301,
    // memory
    SYSCALL_MEMORY_MALLOC = 2400,
    SYSCALL_MEMORY_FREE = 2401,
    SYSCALL_MEMORY_REALLOC = 2402,
    SYSCALL_MAX
} syscalls;

typedef uint64_t (*syscall_handler)(uint64_t syscall_num, void* args);

uint64_t invalid_syscall(uint64_t syscall_id, void* args);

#endif