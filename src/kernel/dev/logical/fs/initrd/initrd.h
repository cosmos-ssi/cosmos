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

#define INITRD_NAME_SIZE 64
#define INITRD_MAX_FILES 64

struct device* initrd_attach(struct device* partition_device, uint32_t lba);
void initrd_detach(struct device* dev);

uint64_t initrd_lba();

#endif