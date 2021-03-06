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
    return ret;
}

void dynabuffer_delete(struct dynabuffer* db) {
    ASSERT_NOT_NULL(db);
    ASSERT_NOT_NULL(db->data);
    ASSERT_NOT_NULL(db->size);
    kfree(db->data);
    kfree(db);
}
