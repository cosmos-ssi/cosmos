//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_parallel.h>
#include <tests/dev/test_parallel.h>

void test_parallel() {
    uint8_t devicename[] = {"par0"};

    struct object* parallel = objectmgr_find_object(devicename);
    if (0 != parallel) {
        struct objecttype_parallel* parallel_api = (struct objecttype_parallel*)parallel->api;

        (*parallel_api->write)(parallel, "hello", 6);

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
