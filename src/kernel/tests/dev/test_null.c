//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_null.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_null.h>

void test_null() {
    kprintf("Testing null\n");

    uint8_t devicename[] = {"null0"};

    struct device* null_ = devicemgr_find_device(devicename);
    if (0 != null_) {
        struct deviceapi_null* null_api = (struct deviceapi_null*)null_->api;

        uint8_t n = (*null_api->read)(null_);
        ASSERT(0 == n);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
