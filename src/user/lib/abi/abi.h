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
uint64_t syscall_process_exit(uint64_t ret);
uint64_t syscall_process_sleep(uint64_t time);

// console
uint64_t syscall_console_write(uint64_t handle, uint8_t* str);

// serial (these apis need to be changed to pass a serial device handle, some day.....)
uint64_t syscall_serial_writechar(uint64_t object, uint64_t c);
uint64_t syscall_serial_readchar(uint64_t object);

// memory
uint64_t syscall_memory_malloc(uint64_t size);
uint64_t syscall_memory_free(void* mem);
uint64_t syscall_memory_realloc(void* mem, uint64_t size);

// object manager
uint64_t syscall_objectmgr_get_device_by_name(const char* name);
uint64_t syscall_objectmgr_get_device_by_handle(uint64_t handle);

// keyboard
uint64_t syscall_keyboard_read(uint64_t object);

struct syscall_objectinterface_resolution {
    uint32_t width;
    uint32_t height;
    uint32_t color_depth;
};

// bga
uint64_t syscall_bga_get_resolution(uint64_t object, struct syscall_objectinterface_resolution* res);
uint64_t syscall_bga_set_resolution(uint64_t object, struct syscall_objectinterface_resolution* res);
uint64_t syscall_bga_get_buffersize(uint64_t object);
uint64_t syscall_bga_blt(uint64_t object, uint8_t* buffer, uint64_t size);

#endif