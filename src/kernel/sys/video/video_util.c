//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_bga.h>
#include <sys/video/video_util.h>

void video_get_resolution(struct object* obj, struct objectinterface_resolution* resolution) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT_NOT_NULL(resolution);
    ASSERT(obj->objectype == BGA);
    struct objectinterface_bga* api = (struct objectinterface_bga*)obj->api;
    if (0 != api->get_resolution) {
        (*api->get_resolution)(obj, resolution);
    }
}

void video_set_resolution(struct object* obj, struct objectinterface_resolution* resolution) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT(obj->objectype == BGA);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT_NOT_NULL(resolution);
    ASSERT(obj->objectype == BGA);
    struct objectinterface_bga* api = (struct objectinterface_bga*)obj->api;
    if (0 != api->set_resolution) {
        (*api->set_resolution)(obj, resolution);
    }
}

uint32_t video_util_get_buffersize(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT(obj->objectype == BGA);
    struct objectinterface_bga* api = (struct objectinterface_bga*)obj->api;
    if (0 != api->get_buffersize) {
        return (*api->get_buffersize)(obj);
    }
    return 0;
}

void video_util_blt(struct object* obj, uint8_t* buffer, uint32_t buffer_size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->api);
    ASSERT(obj->objectype == BGA);
    struct objectinterface_bga* api = (struct objectinterface_bga*)obj->api;
    if (0 != api->blt) {
        (*api->blt)(obj, buffer, buffer_size);
    }
}
