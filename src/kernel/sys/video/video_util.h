//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* wrapper for devices that implement deviceapi_bga
*/
#ifndef _VIDEO_UTIL_H
#define _VIDEO_UTIL_H

#include <types.h>

struct object;
struct objecttype_resolution;

void video_get_resolution(struct object* dev, struct objecttype_resolution* resolution);
void video_set_resolution(struct object* dev, struct objecttype_resolution* resolution);
uint32_t video_util_get_buffersize(struct object* dev);
void video_util_blt(struct object* dev, uint8_t* buffer, uint32_t buffer_size);

#endif
