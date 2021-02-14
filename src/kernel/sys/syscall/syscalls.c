//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/deviceapi/deviceapi_console.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/syscall/syscalls.h>

// syscall 1
uint64_t syscall_print_console(uint64_t syscall_id) {
    struct device* console_dev = devicemgr_find_device("console0");
    if (0 != console_dev) {
        struct deviceapi_console* api = (struct deviceapi_console*)console_dev->api;
        (*api->write)(console_dev, "Hello fron console\n");
    }
    return 0;
}
