//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <../lib/types.h>

// syscall numbers are defined in userland.md, syscall.h (userland) and syscall.h (kernel)
// process
#define SYSCALL_PROCESS_SLEEP 1100
#define SYSCALL_PROCESS_EXIT 1101
// bga
#define SYSCALL_BGA_GETRESOLUTION 1200
#define SYSCALL_BGA_SETRESOLUTION 1201
#define SYSCALL_BGA_GETBUFFERSIZE 1202
#define SYSCALL_BGA_BLT 1203
// console
#define SYSCALL_CONSOLE_WRITE 1401
// keyboard
#define SYSCALL_KEYBOARD_READ 1600
// tick
#define SYSCALL_TICK_READ 2200
// serial
#define SYSCALL_SERIAL_READCHAR 2300
#define SYSCALL_SERIAL_WRITECHAR 2301
// memory
#define SYSCALL_MEMORY_MALLOC 2400
#define SYSCALL_MEMORY_FREE 2401
#define SYSCALL_MEMORY_REALLOC 2402
// hostid
#define SYSCALL_HOSTID_GETID 2700
// object mgr
#define SYSCALL_OBJMGR_GET_DEVICE_BY_NAME 2800
#define SYSCALL_OBJMGR_GET_DEVICE_BY_HANDLE 2801

uint64_t syscall(uint64_t syscall, uint64_t param1, uint64_t param2, uint64_t param3);

#endif
