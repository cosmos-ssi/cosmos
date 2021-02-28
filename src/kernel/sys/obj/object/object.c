//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/object/object.h>

uint64_t object_handle_count = 0;

struct object* object_new_object() {
    struct object* ret = (struct object*)kmalloc(sizeof(struct object));
    for (uint16_t i = 0; i < OBJECT_MAX_DESCRIPTION; i++) {
        ret->description[i] = 0;
    }
    object_handle_count += 1;
    ret->handle = object_handle_count;
    ret->init = 0;
    ret->object_data = 0;
    ret->name = 0;
    ret->type_index = 0;
    ret->objectype = 0;
    ret->api = 0;
    ret->pci = 0;
    ret->reference_count = 0;

    return ret;
}