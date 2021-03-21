//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>

uint64_t syscall_serial_readchar(uint64_t syscall_id, void* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}
uint64_t syscall_serial_writechar(uint64_t syscall_id, void* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}
