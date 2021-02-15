//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <types.h>

uint64_t syscall_print_console(uint64_t syscall_id, void* args);
uint64_t syscall_exit(uint64_t syscall_id, void* args);
uint64_t invalid_syscall(uint64_t syscall_id, void* args);

#endif