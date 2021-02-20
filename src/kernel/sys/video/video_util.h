//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VIDEO_UTIL_H
#define _VIDEO_UTIL_H

#include <types.h>

struct device;

void video_util_clear(struct device* dev, uint32_t color);
uint32_t video_util_get_width_function(struct device* dev);
uint32_t video_util_get_height_function(struct device* dev);
uint32_t video_util_get_colordepth_function(struct device* dev);
uint32_t video_util_get_buffersize(struct device* dev);
void video_util_blt(struct device* dev, uint8_t* buffer, uint32_t buffer_size);

#endif
