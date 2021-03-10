//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* one could reasonably ask why we need a tick device when we have pit.  
* pit is platform dependent while tick is not.
* we can have a slighly differnet implementation of pit on ARM that tick uses, and wrap it.
*/
#ifndef _TICK_H
#define _TICK_H

#include <types.h>

struct object;

struct object* tick_attach(struct object* pit_device);
void tick_detach(struct object* obj);

#endif