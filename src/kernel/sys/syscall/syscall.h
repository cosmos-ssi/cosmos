/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

typedef enum syscalls { SYSCALL_EXIT = 0, SYSCALL_MAX } syscalls;

extern uint64_t (*syscall_table[SYSCALL_MAX])(void* args);

// syscall_dispatcher.c
void invalid_syscall();
void syscall_dispatcher_init();

#endif