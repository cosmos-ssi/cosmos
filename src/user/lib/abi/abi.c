//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/abi.h>
#include <abi/syscall.h>

uint64_t syscall_process_exit(uint64_t ret) {
    struct syscall_args args;
    args.arg1 = ret;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_PROCESS_EXIT, &args);
}

uint64_t syscall_process_sleep(uint64_t time) {
    struct syscall_args args;
    args.arg1 = time;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_PROCESS_SLEEP, &args);
}

uint64_t syscall_console_write(uint64_t handle, uint8_t* str) {
    struct syscall_args args;
    args.arg1 = handle;
    args.arg2 = (uint64_t)str;
    args.arg3 = 0;
    return syscall(SYSCALL_CONSOLE_WRITE, &args);
}

uint64_t syscall_memory_malloc(uint64_t size) {
    struct syscall_args args;
    args.arg1 = size;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_MEMORY_MALLOC, &args);
}

uint64_t syscall_memory_free(void* mem) {
    struct syscall_args args;
    args.arg1 = (uint64_t)mem;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_MEMORY_FREE, &args);
}

uint64_t syscall_memory_realloc(void* mem, uint64_t size) {
    struct syscall_args args;
    args.arg1 = (uint64_t)mem;
    args.arg2 = size;
    args.arg3 = 0;
    return syscall(SYSCALL_MEMORY_REALLOC, &args);
}

uint64_t syscall_serial_writechar(uint64_t object, uint64_t c) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = (uint64_t)c;
    args.arg3 = 0;
    return syscall(SYSCALL_SERIAL_WRITECHAR, &args);
}
uint64_t syscall_serial_readchar(uint64_t object) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_SERIAL_READCHAR, &args);
}

uint64_t syscall_objectmgr_get_device_by_name(const char* name) {
    struct syscall_args args;
    args.arg1 = (uint64_t)name;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_OBJMGR_GET_DEVICE_BY_NAME, &args);
}

uint64_t syscall_objectmgr_get_device_by_handle(uint64_t handle) {
    struct syscall_args args;
    args.arg1 = handle;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_OBJMGR_GET_DEVICE_BY_HANDLE, &args);
}

uint64_t syscall_keyboard_read(uint64_t object) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_KEYBOARD_READ, &args);
}

uint64_t syscall_bga_get_resolution(uint64_t object, struct syscall_objectinterface_resolution* res) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = (uint64_t)res;
    args.arg3 = 0;
    return syscall(SYSCALL_BGA_GETRESOLUTION, &args);
}

uint64_t syscall_bga_set_resolution(uint64_t object, struct syscall_objectinterface_resolution* res) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = (uint64_t)res;
    args.arg3 = 0;
    return syscall(SYSCALL_BGA_SETRESOLUTION, &args);
}

uint64_t syscall_bga_get_buffersize(uint64_t object) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = 0;
    args.arg3 = 0;
    return syscall(SYSCALL_BGA_GETBUFFERSIZE, &args);
}

uint64_t syscall_bga_blt(uint64_t object, uint8_t* buffer, uint64_t size) {
    struct syscall_args args;
    args.arg1 = object;
    args.arg2 = (uint64_t)buffer;
    args.arg3 = size;
    return syscall(SYSCALL_BGA_BLT, &args);
}
