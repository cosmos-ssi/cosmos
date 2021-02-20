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
#include <sys/video/rgb.h>
#include <sys/video/video_util.h>

void video_util_clear(struct device* dev, uint32_t rgb) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    /*
    * components
    */
    uint8_t r = rgb_r(rgb);
    uint8_t g = rgb_g(rgb);
    uint8_t b = rgb_b(rgb);
    kprintf("rgb %#llX, r %#llX, g %#llX, b %#llX\n", rgb, r, g, b);
    /*
    * sizes
    */
    uint32_t width = video_util_get_width_function(dev);
    ASSERT(width == 800);
    uint32_t height = video_util_get_height_function(dev);
    ASSERT(height == 600);
    uint32_t colordepth = video_util_get_colordepth_function(dev);
    ASSERT(colordepth == 24);
    /*
    * make a buffer
    */
    uint32_t buffer_size = video_util_get_buffersize(dev);
    uint8_t* buffer = kmalloc(buffer_size);
    /*
    * paint
    */
    for (uint32_t i = 0; i < (width * height * 3); i += 3) {
        buffer[i] = b;
        buffer[i + 1] = g;
        buffer[i + 2] = r;
    }
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
