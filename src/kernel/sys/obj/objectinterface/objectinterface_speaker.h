//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all speaker objects will implement
 */
#ifndef _OBJECTTYPE_SPEAKER_H
#define _OBJECTTYPE_SPEAKER_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*speaker_beep_function)(struct object* obj, uint32_t frequency, uint32_t milliseconds);

struct objectinterface_speaker {
    speaker_beep_function beep;
};

#endif