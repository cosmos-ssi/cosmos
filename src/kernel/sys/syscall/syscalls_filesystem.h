//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_FILESYSTEM_H
#define _SYSCALLS_FILESYSTEM_H

#include <sys/syscall/syscall.h>
#include <types.h>

uint64_t syscall_filesystem_get_root_node(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_filesystem_read(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_filesystem_write(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_filesystem_open(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_filesystem_close(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_filesystem_find_node_by_id(uint64_t syscall_id, struct syscall_args* args);
uint64_t syscall_filesystem_list_directory(uint64_t syscall_id, struct syscall_args* args);

#endif