//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_nic.h>
#include <tests/dev/test_rtl8139.h>

void test_rtl8139() {
    uint8_t devicename[] = {"nic0"};

    struct object* ethernet = objectmgr_find_object(devicename);
    if (0 != ethernet) {
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}