//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* file utilties
*/
#ifndef _FILE_UTIL_H
#define _FILE_UTIL_H

#include <types.h>

struct filesystem_node;
/*
* read a file. this kmallocs new memory and returns it.  The file len is populated into len
*/
uint8_t* file_util_read_file(uint8_t* device_name, uint8_t* file_name, uint32_t* len);

struct filesystem_node* file_util_find_file(uint8_t* device_name, uint8_t* file_name);

#endif