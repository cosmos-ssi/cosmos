//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* http://www.jamesmolloy.co.uk/tutorial_html/8.-The%20VFS%20and%20the%20initrd.html
*/
#ifndef _INITRD_H
#define _INITRD_H

#define INITRD_DISK "disk0"

#include <types.h>

struct object;

#define INITRD_NAME_SIZE 64
#define INITRD_MAX_FILES 64

struct object* initrd_attach(struct object* partition_object, uint32_t lba);
void initrd_detach(struct object* obj);

uint64_t initrd_lba();

#endif