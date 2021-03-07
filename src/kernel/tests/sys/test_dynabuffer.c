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

    uint8_t a = 12;
    uint8_t b = 17;
    uint8_t c = 120;
    uint16_t d = 0x1000;
    uint16_t e = 0x1007;
    uint32_t f = 0x44441007;
    uint32_t g = 0x99991007;
    uint64_t h = 0x1234567899991007;

    dynabuffer_append_uint8_t(db, a);
    dynabuffer_append_uint8_t(db, b);
    dynabuffer_append_uint8_t(db, c);
    dynabuffer_append_uint16_t(db, d);
    dynabuffer_append_uint16_t(db, e);
    dynabuffer_append_uint32_t(db, f);
    dynabuffer_append_uint32_t(db, g);
    dynabuffer_append_uint64_t(db, h);

    //  debug_show_memblock(db->data, db->size);

    dynabuffer_reset(db);
    ASSERT(dynabuffer_read_uint8_t(db) == 12);
    ASSERT(dynabuffer_read_uint8_t(db) == 17);
    ASSERT(dynabuffer_read_uint8_t(db) == 120);
    ASSERT(dynabuffer_read_uint16_t(db) == 0x1000);
    ASSERT(dynabuffer_read_uint16_t(db) == 0x1007);
    ASSERT(dynabuffer_read_uint32_t(db) == 0x44441007);
    ASSERT(dynabuffer_read_uint32_t(db) == 0x99991007);
    ASSERT(dynabuffer_read_uint64_t(db) == 0x1234567899991007);

    dynabuffer_delete(db);
}
