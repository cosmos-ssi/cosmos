//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/dynabuffer/dynabuffer.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <tests/sys/test_dynabuffer.h>
#include <types.h>

void test_dynabuffer() {
    kprintf("Testing Dynabuffer\n");

    struct dynabuffer* db = dynabuffer_new();
    ASSERT_NOT_NULL(db);
    dynabuffer_delete(db);
}
