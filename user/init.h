//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* this is the init program, which cosmos will read from disk, and execute in user-land context to kick off userland code
* this program will communicate with the kernel via ABI calls
*/

#ifndef _INIT_H
#define _INIT_H

void cosmos_userland_init();

#endif
