//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_null.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_null.h>
#include <types.h>

void test_null() {
    kprintf("Testing null\n");

    uint8_t devicename[] = {"null0"};

    struct object* null_ = objectmgr_find_object(devicename);
    if (0 != null_) {
        struct objectinterface_null* null_api = (struct objectinterface_null*)null_->api;

        uint8_t n = (*null_api->read)(null_);
        ASSERT(0 == n);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
