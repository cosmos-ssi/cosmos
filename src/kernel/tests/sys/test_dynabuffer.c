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
#include <sys/string/string.h>
#include <tests/sys/test_dynabuffer.h>
#include <types.h>

#define DYNABUFFER_TEST_DAVES "cat"

void test_dynabuffer_atomic_types() {
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

void test_dynabuffer_byte_arrays() {
    struct dynabuffer* db = dynabuffer_new();
    ASSERT_NOT_NULL(db);

    uint8_t a1[4] = {0x09, 0x07, 0x05, 0x03};
    uint8_t a2[4] = {0x00, 0x00, 0x00, 0x00};

    dynabuffer_append_bytes(db, a1, 4);
    ASSERT(db->data[0] == 0x09);
    ASSERT(db->data[1] == 0x07);
    ASSERT(db->data[2] == 0x05);
    ASSERT(db->data[3] == 0x03);
    ASSERT(db->idx == 4);

    dynabuffer_reset(db);
    ASSERT(db->idx == 0);

    dynabuffer_read_bytes(db, a2, 4);
    ASSERT(a2[0] == 0x09);
    ASSERT(a2[1] == 0x07);
    ASSERT(a2[2] == 0x05);
    ASSERT(a2[3] == 0x03);
    ASSERT(db->idx == 4);
    dynabuffer_delete(db);
}

void test_dynabuffer_strings() {
    uint8_t buffer[32];
    struct dynabuffer* db = dynabuffer_new();
    ASSERT_NOT_NULL(db);
    dynabuffer_append_string(db, DYNABUFFER_TEST_DAVES);
    dynabuffer_reset(db);
    dynabuffer_read_string(db, buffer, 32);
    ASSERT(strcmp(DYNABUFFER_TEST_DAVES, buffer) == 0);
    dynabuffer_delete(db);
}

void test_dynabuffer() {
    kprintf("Testing Dynabuffer\n");
    test_dynabuffer_atomic_types();
    test_dynabuffer_byte_arrays();
    test_dynabuffer_strings();
}