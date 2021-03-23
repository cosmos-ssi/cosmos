//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* cosmos syscalls
*/

#ifndef _ABI_H
#define _ABI_H

#include <types.h>

// process
uint64_t syscall_process_exit();
uint64_t syscall_process_sleep(uint64_t time);

// console
uint64_t syscall_console_write(uint8_t* str);

// serial (these apis need to be changed to pass a serial device handle, some day.....)
uint64_t syscall_serial_writechar(uint64_t c);
uint64_t syscall_serial_readchar();

// memory
uint64_t syscall_memory_malloc(uint64_t size);
uint64_t syscall_memory_free(void* mem);
uint64_t syscall_memory_realloc(void* mem, uint64_t size);

// object manager
uint64_t syscall_objectmgr_get_device_by_name(uint8_t* name);
uint64_t syscall_objectmgr_get_device_by_handle(uint64_t handle);

#endif