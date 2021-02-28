//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/array/array.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/objecttypes.h>

struct array* types;

void objecttypes_init() {
    //   kprintf("Init Device Types\n");
    types = array_new(MAX_DEVICE_TYPES);
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        array_set(types, i, 0);
    }
}

struct arraylist* objecttypes_get_objectlist(device_type dt) {
    ASSERT_NOT_NULL(types);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        return (struct arraylist*)array_get(types, dt);
    } else {
        PANIC("Invalid device type passed to objecttypes_get_objectlist");
    }
    return 0;
}

void objecttypes_set_objectlist(device_type dt, struct arraylist* lst) {
    ASSERT_NOT_NULL(types);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        array_set(types, dt, lst);
    } else {
        PANIC("Invalid device type passed to objecttypes_set_objectlist");
    }
}

uint32_t objecttypes_count() {
    uint32_t ret = 0;
    for (uint32_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        if (0 != array_get(types, i)) {
            ret += 1;
        }
    }
    return ret;
}
