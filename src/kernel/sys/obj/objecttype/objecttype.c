//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/string.h>

struct object_type* objecttype_new(uint8_t* name, enum object_type_id id) {
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(id);
    ASSERT(strlen(name) < OBJECT_TYPE_MAX_NAME);
    struct object_type* ret = (struct object_type*)kmalloc(sizeof(struct object_type));
    strncpy(ret->name, name, OBJECT_TYPE_MAX_NAME);
    ret->id = id;
    return ret;
}
