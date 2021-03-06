//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/smbios/smbios.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_serializer.h>
#include <types.h>

void test_serializer() {
    kprintf("Testing Serializer\n");
    uint8_t devicename[] = {"serializer0"};

    /*
     * find the serializer
     */
    struct object* serializer = objectmgr_find_object_by_name(devicename);
    if (0 != serializer) {
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
