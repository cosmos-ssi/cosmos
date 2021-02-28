//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/objectmgr/object.h>
#include <sys/objecttype/objecttype_bga.h>
#include <sys/video/video_util.h>

void video_get_resolution(struct object* dev, struct objecttype_resolution* resolution) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(resolution);
    ASSERT(dev->devicetype == BGA);
    struct objecttype_bga* api = (struct objecttype_bga*)dev->api;
    if (0 != api->get_resolution) {
        (*api->get_resolution)(dev, resolution);
    }
}

void video_set_resolution(struct object* dev, struct objecttype_resolution* resolution) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(resolution);
    ASSERT(dev->devicetype == BGA);
    struct objecttype_bga* api = (struct objecttype_bga*)dev->api;
    if (0 != api->set_resolution) {
        (*api->set_resolution)(dev, resolution);
    }
}

uint32_t video_util_get_buffersize(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct objecttype_bga* api = (struct objecttype_bga*)dev->api;
    if (0 != api->get_buffersize) {
        return (*api->get_buffersize)(dev);
    }
    return 0;
}

void video_util_blt(struct object* dev, uint8_t* buffer, uint32_t buffer_size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct objecttype_bga* api = (struct objecttype_bga*)dev->api;
    if (0 != api->blt) {
        (*api->blt)(dev, buffer, buffer_size);
    }
}
