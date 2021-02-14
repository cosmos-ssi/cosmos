//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALL_ROUTER_H
#define _SYSCALL_ROUTER_H

#define SYSCALL_MAX_SYSCALLS 64

#include <sys/syscall/syscalls.h>
#include <types.h>

uint64_t syscall_router_route(uint64_t syscall_id);
void syscall_router_init();
void syscall_router_add(uint64_t syscall_id, syscall_handler handler);
#endif