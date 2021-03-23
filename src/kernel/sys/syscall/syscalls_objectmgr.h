//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALLS_OBJMGR_H
#define _SYSCALLS_OBJMGR_H

#include <sys/obj/object/object.h>
#include <types.h>

uint64_t syscall_objectmgr_get_device_by_name(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3);
uint64_t syscall_objectmgr_get_device_by_handle(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3);

#endif