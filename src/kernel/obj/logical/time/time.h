//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* one could reasonably ask why we need a time device when we have rtc.  
* time is platform dependent while rtc is not.
* we can have a slighly differnet implementation of rtc on ARM that time uses, and wrap it.
*/
#ifndef _TIME_H
#define _TIME_H

#include <types.h>

struct object;

struct object* time_attach(struct object* rtc_device);
void time_detach(struct object* obj);

#endif