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

struct device* initrd_attach(struct device* partition_device, uint32_t lba);
void initrd_detach(struct device* dev);

uint8_t initrd_get_file_name(struct device* initrd_dev, uint8_t idx, uint8_t* name, uint16_t size);
uint8_t initrd_get_file_size(struct device* initrd_dev, uint8_t idx, uint16_t* size);
uint8_t initrd_get_file_data(struct device* initrd_dev, uint8_t idx, uint8_t* data, uint32_t size);
void initrd_dump_dir(struct device* initrd_dev);
uint64_t initrd_lba();

#endif