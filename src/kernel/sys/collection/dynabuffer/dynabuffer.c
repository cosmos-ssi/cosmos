//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/dynabuffer/dynabuffer.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>

#define DYNABUFFER_DEFAULT_SIZE 255

struct dynabuffer* dynabuffer_new() {
    struct dynabuffer* ret = (struct dynabuffer*)kmalloc(sizeof(struct dynabuffer));
    ret->size = DYNABUFFER_DEFAULT_SIZE;
    ret->data = kmalloc(ret->size);
    ret->idx = 0;
    return ret;
}

void dynabuffer_delete(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    ASSERT_NOT_NULL(db->size);
    kfree(db->data);
    kfree(db);
}

void dynabuffer_append_uint8_t(struct dynabuffer* db, uint8_t v) {
    ASSERT_NOT_NULL(db);
}
void dynabuffer_append_uint16_t(struct dynabuffer* db, uint16_t v) {
    ASSERT_NOT_NULL(db);
}
void dynabuffer_append_uint32_t(struct dynabuffer* db, uint32_t v) {
    ASSERT_NOT_NULL(db);
}
void dynabuffer_append_uint64_t(struct dynabuffer* db, uint64_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(v);
}

void dynabuffer_read_uint8_t(struct dynabuffer* db, uint8_t* v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(v);
}
void dynabuffer_read_uint16_t(struct dynabuffer* db, uint16_t* v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(v);
}
void dynabuffer_read_uint32_t(struct dynabuffer* db, uint32_t* v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(v);
}
void dynabuffer_read_uint64_t(struct dynabuffer* db, uint64_t* v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(v);
}
