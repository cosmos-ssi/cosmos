//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all DSP devices will implement
 */
#ifndef _OBJECTTYPE_DSP_H
#define _OBJECTTYPE_DSP_H

#include <sys/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*dsp_play_function)(struct object* obj, uint8_t* buffer, uint16_t rate, uint8_t depth, uint8_t channels,
                                  uint64_t len);

struct objectinterface_dsp {
    dsp_play_function play;
};

#endif