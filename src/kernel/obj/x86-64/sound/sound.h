//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SOUND_H
#define _SOUND_H

#include <obj/x86-64/sound/ac97/ac97.h>
#include <obj/x86-64/sound/adlib/adlib.h>
#include <obj/x86-64/sound/sb16/sb16.h>

void sound_objectmgr_register_objects();

#endif
