/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

// syscall numbers are defined in userland.md, syscall.h (userland) and syscall.h (kernel)
typedef enum syscalls {
    // process
    SYSCALL_PROCESS_EXIT = 1101,
    SYSCALL_PROCESS_SLEEP = 1100,
    // bga
    SYSCALL_BGA_GET_RESOLUTION = 1200,
    SYSCALL_BGA_SET_RESOLUTION = 1201,
    SYSCALL_BGA_GET_BUFFERSIZE = 1202,
    SYSCALL_BGA_BLT = 1203,
    // console
    SYSCALL_CONSOLE_WRITE = 1401,
    // filesystem
    SYSCALL_FILESYSTEM_GET_ROOT_NODE = 1500,
    SYSCALL_FILESYSTEM_READ = 1501,
    SYSCALL_FILESYSTEM_WRITE = 1502,
    SYSCALL_FILESYSTEM_OPEN = 1503,
    SYSCALL_FILESYSTEM_CLOSE = 1504,
    SYSCALL_FILESYSTEM_FIND_NODE_BY_ID = 1505,
    SYSCALL_FILESYSTEM_LIST_DIRECTORY = 1506,
    // keyboard
    SYSCALL_KEYBOARD_READ = 1600,
    // serial
    SYSCALL_SERIAL_READCHAR = 2300,
    SYSCALL_SERIAL_WRITECHAR = 2301,
    // memory
    SYSCALL_MEMORY_MALLOC = 2400,
    SYSCALL_MEMORY_FREE = 2401,
    SYSCALL_MEMORY_REALLOC = 2402,
    // object mgr
    SYSCALL_OBJMGR_GET_DEVICE_BY_NAME = 2800,
    SYSCALL_OBJMGR_GET_DEVICE_BY_HANDLE = 2801,
    // max
    SYSCALL_MAX
} syscalls;

struct syscall_args {
    uint64_t arg1;
    uint64_t arg2;
    uint64_t arg3;
} __attribute__((packed));

typedef uint64_t (*syscall_handler)(uint64_t syscall_num, struct syscall_args* args);

uint64_t invalid_syscall(uint64_t syscall_id, struct syscall_args* args);

#endif