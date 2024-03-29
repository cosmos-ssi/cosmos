//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_kernelmap.h>

void test_kernelmap() {
    kprintf("Testing Kernelmap\n");

    struct object* obj = objectmgr_find_object_by_name("kernelmap0");
    ASSERT_NOT_NULL(obj);
}
