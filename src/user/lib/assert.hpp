//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ASSERT_HPP
#define _ASSERT_HPP

extern "C" {
#include <abi/abi.h>
}

// assertion macros to make life easier
// in a RELEASE build these can just expand to nothing
#define ASSERT(condition)                                                                                              \
    if (false == (condition)) {                                                                                        \
        uint64_t con0_device_handle = syscall_objectmgr_get_device_by_name("con0");                                    \
        syscall_console_write(con0_device_handle, (uint8_t*)"assertion failure\n");                                    \
    }

#define ASSERT_NOT_NULL(term)                                                                                          \
    if (0 == (term)) {                                                                                                 \
        uint64_t con0_device_handle = syscall_objectmgr_get_device_by_name("con0");                                    \
        syscall_console_write(con0_device_handle, (uint8_t*)"assertion failure\n");                                    \
    }

#endif