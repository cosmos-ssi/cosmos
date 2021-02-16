//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* cosmos syscalls
*/

#ifndef _COSMOS_ABI_H
#define _COSMOS_ABI_H

#include <rootfs/lib/cosmos_types.h>

uint64_t syscall_exit();
uint64_t syscall_print_console();
uint64_t syscall_malloc(uint64_t size);
uint64_t syscall_free(void* mem);
uint64_t syscall_realloc(void* mem, uint64_t size);
uint64_t syscall_sleep(uint64_t time);
#endif