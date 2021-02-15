/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

typedef enum syscalls { SYSCALL_EXIT = 0, SYSCALL_PRINT_CONSOLE = 1, SYSCALL_MAX } syscalls;
typedef uint64_t (*syscall_handler)(uint64_t syscall_num, void* args);

// syscall_dispatcher.c
void syscall_add(uint64_t syscall_num, syscall_handler handler);
void syscall_dispatcher_init();

#endif