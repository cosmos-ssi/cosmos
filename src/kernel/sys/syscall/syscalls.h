//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <types.h>

typedef uint64_t (*syscall_handler)(uint64_t syscall_id);

// syscall 1
uint64_t syscall_print_console(uint64_t syscall_id);

#endif