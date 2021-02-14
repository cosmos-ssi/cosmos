//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicetypes.h>
#include <sys/collection/array/array.h>
#include <sys/panic/panic.h>

struct arraylist;

struct array* types;

void devicetypes_init() {
    //   kprintf("Init Device Types\n");
    types = array_new(MAX_DEVICE_TYPES);
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        array_set(types, i, 0);
    }
}

struct arraylist* devicetypes_get_devicelist(device_type dt) {
    ASSERT_NOT_NULL(types);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        return (struct arraylist*)array_get(types, dt);
    } else {
        PANIC("Invalid device type passed to devicetypes_get_devicelist");
    }
    return 0;
}

void devicetypes_set_devicelist(device_type dt, struct arraylist* lst) {
    ASSERT_NOT_NULL(types);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        array_set(types, dt, lst);
    } else {
        PANIC("Invalid device type passed to devicetypes_set_devicelist");
    }
}

uint32_t devicetypes_count() {
    uint32_t ret = 0;
    for (uint32_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        if (0 != array_get(types, i)) {
            ret += 1;
        }
    }
    return ret;
}
