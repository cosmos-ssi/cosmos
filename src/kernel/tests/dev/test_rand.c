//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_rand.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_rand.h>
#include <types.h>

void test_rand() {
    kprintf("Testing rand\n");

    struct device* rand = devicemgr_find_device("rand0");
    if (0 != rand) {
        struct deviceapi_rand* rand_api = (struct deviceapi_rand*)rand->api;
        uint64_t last = 0;
        for (uint64_t i = 0; i < 100; i++) {
            uint64_t r = (*rand_api->read)(rand);
            ASSERT(r != last);
            last = r;
        }
    } else {
        kprintf("Unable to find rand0\n");
    }
}