//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/dynabuffer/dynabuffer.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

#define DYNABUFFER_DEFAULT_SIZE 255

struct dynabuffer* dynabuffer_new() {
    struct dynabuffer* ret = (struct dynabuffer*)kmalloc(sizeof(struct dynabuffer));
    ret->size = DYNABUFFER_DEFAULT_SIZE;
    ret->data = kmalloc(ret->size);
    memzero(ret->data, ret->size);
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

void dynabuffer_reset(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    db->idx = 0;
}

uint32_t dynabuffer_size(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    return db->size;
}
uint32_t dynabuffer_idx(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    return db->idx;
}

void dynabuffer_append_uint8_t(struct dynabuffer* db, uint8_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx + sizeof(uint8_t) < db->size);
    db->data[db->idx] = v;
    db->idx += sizeof(uint8_t);
}
void dynabuffer_append_uint16_t(struct dynabuffer* db, uint16_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx + sizeof(uint16_t) < db->size);
    ((uint16_t*)db->data)[db->idx] = v;
    db->idx += sizeof(uint16_t);
}
void dynabuffer_append_uint32_t(struct dynabuffer* db, uint32_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx + sizeof(uint32_t) < db->size);
    ((uint32_t*)db->data)[db->idx] = v;
    db->idx += sizeof(uint32_t);
}
void dynabuffer_append_uint64_t(struct dynabuffer* db, uint64_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx + sizeof(uint64_t) < db->size);
    ASSERT_NOT_NULL(v);
    ((uint64_t*)db->data)[db->idx] = v;
    db->idx += sizeof(uint64_t);
}

uint8_t dynabuffer_read_uint8_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx < db->size - sizeof(uint8_t));
    uint8_t ret = db->data[db->idx];
    db->idx += sizeof(uint8_t);
    return ret;
}
uint16_t dynabuffer_read_uint16_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx < db->size - sizeof(uint16_t));
    uint16_t ret = ((uint16_t*)db->data)[db->idx];
    db->idx += sizeof(uint16_t);
    return ret;
}

uint32_t dynabuffer_read_uint32_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx < db->size - sizeof(uint32_t));
    uint32_t ret = ((uint32_t*)db->data)[db->idx];
    db->idx += sizeof(uint32_t);
    return ret;
}
uint64_t dynabuffer_read_uint64_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT(db->idx < db->size - sizeof(uint64_t));
    uint64_t ret = ((uint64_t*)db->data)[db->idx];
    db->idx += sizeof(uint64_t);
    return ret;
}
