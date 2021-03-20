//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/gui/props.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_props.h>

void test_props() {
    kprintf("Testing props\n");

    struct props* p = props_load("fs0", "test_p~1.txt");
    ASSERT_NOT_NULL(p);

    uint8_t* one = props_get(p, "a");
    ASSERT_NOT_NULL(one);
    ASSERT(0 == strcmp(one, "b"));
    uint8_t* two = props_get(p, "a.b");
    ASSERT_NOT_NULL(two);
    ASSERT(0 == strcmp(two, "123"));
    uint8_t* three = props_get(p, "a.b.c");
    ASSERT_NOT_NULL(three);
    ASSERT(0 == strcmp(three, "0x000102"));
}
