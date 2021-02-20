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

void video_util_clear(struct device* dev, uint32_t color) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    /*
    * make a buffer
    */
    uint32_t buffer_size = video_util_get_buffersize(dev);
    uint8_t* buffer = kmalloc(buffer_size);
    memset(buffer, (uint8_t)color, buffer_size);
    /*
    * blt
    */
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->blt) {
        (*api->blt)(dev, buffer, buffer_size);
    }
    /*
    * done
    */
    kfree(buffer);
}

uint32_t video_util_get_width_function(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_width) {
        return (*api->get_width)(dev);
    }
    return 0;
}
uint32_t video_util_get_height_function(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_height) {
        return (*api->get_height)(dev);
    }
    return 0;
}
uint32_t video_util_get_colordepth_function(struct device* dev) {
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
