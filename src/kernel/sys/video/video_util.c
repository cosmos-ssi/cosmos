//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_bga.h>
#include <sys/devicemgr/device.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/video/video_util.h>

uint32_t video_util_get_width(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_width) {
        return (*api->get_width)(dev);
    }
    return 0;
}

uint32_t video_util_get_height(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_height) {
        return (*api->get_height)(dev);
    }
    return 0;
}

uint32_t video_util_get_colordepth(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_colordepth) {
        return (*api->get_colordepth)(dev);
    }
    return 0;
}

uint32_t video_util_get_buffersize(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_buffersize) {
        return (*api->get_buffersize)(dev);
    }
    return 0;
}

void video_util_blt(struct device* dev, uint8_t* buffer, uint32_t buffer_size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->blt) {
        (*api->blt)(dev, buffer, buffer_size);
    }
}
