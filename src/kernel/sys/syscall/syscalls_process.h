//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_PROCESS_H
#define _SYSCALLS_PROCESS_H

#include <types.h>

// process
uint64_t syscall_process_exit(uint64_t syscall_id, void* args);
uint64_t syscall_process_sleep(uint64_t syscall_id, void* args);

#endif