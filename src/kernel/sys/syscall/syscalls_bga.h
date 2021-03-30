//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_BGA_H
#define _SYSCALLS_BGA_H

#include <sys/syscall/syscall.h>
#include <types.h>

uint64_t syscall_bga_get_resolution(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_bga_set_resolution(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_bga_get_buffersize(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_bga_blt(uint64_t syscall_id, struct syscall_args* args);

#endif