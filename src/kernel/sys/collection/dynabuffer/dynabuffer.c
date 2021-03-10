//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/dynabuffer/dynabuffer.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

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
    ASSERT_NOT_NULL(db->data);
    db->idx = 0;
}

uint32_t dynabuffer_size(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    return db->size;
}

uint32_t dynabuffer_idx(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    return db->idx;
}

uint8_t* dynabuffer_data(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    return db->data;
}

void dynabuffer_append_uint8_t(struct dynabuffer* db, uint8_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx + sizeof(uint8_t) < db->size);
    db->data[db->idx] = v;
    db->idx += sizeof(uint8_t);
}

void dynabuffer_append_uint16_t(struct dynabuffer* db, uint16_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx + sizeof(uint16_t) < db->size);
    ((uint16_t*)db->data)[db->idx] = v;
    db->idx += sizeof(uint16_t);
}

void dynabuffer_append_uint32_t(struct dynabuffer* db, uint32_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx + sizeof(uint32_t) < db->size);
    ((uint32_t*)db->data)[db->idx] = v;
    db->idx += sizeof(uint32_t);
}

void dynabuffer_append_uint64_t(struct dynabuffer* db, uint64_t v) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx + sizeof(uint64_t) < db->size);
    ASSERT_NOT_NULL(v);
    ((uint64_t*)db->data)[db->idx] = v;
    db->idx += sizeof(uint64_t);
}

uint8_t dynabuffer_read_uint8_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx < db->size - sizeof(uint8_t));
    uint8_t ret = db->data[db->idx];
    db->idx += sizeof(uint8_t);
    return ret;
}

uint16_t dynabuffer_read_uint16_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx < db->size - sizeof(uint16_t));
    uint16_t ret = ((uint16_t*)db->data)[db->idx];
    db->idx += sizeof(uint16_t);
    return ret;
}

uint32_t dynabuffer_read_uint32_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx < db->size - sizeof(uint32_t));
    uint32_t ret = ((uint32_t*)db->data)[db->idx];
    //   kprintf("idx2 %llu\n", db->idx);
    //   kprintf("ret %llu\n", ret);
    db->idx += sizeof(uint32_t);
    return ret;
}

uint64_t dynabuffer_read_uint64_t(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);

    ASSERT(db->idx < db->size - sizeof(uint64_t));
    uint64_t ret = ((uint64_t*)db->data)[db->idx];
    db->idx += sizeof(uint64_t);
    return ret;
}

void dynabuffer_append_bytes(struct dynabuffer* db, uint8_t* buffer, uint32_t size) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    ASSERT_NOT_NULL(buffer);
    ASSERT_NOT_NULL(size);
    ASSERT((size + (db->idx)) <= db->size);
    memcpy(&(db->data[db->idx]), buffer, size);
    db->idx += size;
}

void dynabuffer_read_bytes(struct dynabuffer* db, uint8_t* buffer, uint32_t size) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    ASSERT_NOT_NULL(buffer);
    ASSERT_NOT_NULL(size);
    ASSERT(((db->size) - (db->idx)) >= size);
    memcpy(buffer, &(db->data[db->idx]), size);
    db->idx += size;
}

/*
* strings are stored pascal-style, with a null termination too
*/
void dynabuffer_append_string(struct dynabuffer* db, uint8_t* str) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    ASSERT_NOT_NULL(str);
    uint32_t len = strlen(str);
    dynabuffer_append_uint32_t(db, len);
    dynabuffer_append_bytes(db, str, len + 1);
}

void dynabuffer_read_string(struct dynabuffer* db, uint8_t* str, uint32_t size) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    ASSERT_NOT_NULL(str);
    uint32_t len = dynabuffer_read_uint32_t(db);
    dynabuffer_read_bytes(db, str, len + 1);
    // check that we did get the null termination
    ASSERT(str[len] == 0);
}
