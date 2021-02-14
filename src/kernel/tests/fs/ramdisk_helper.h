//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef __RAMDISK_HELPER_H
#define __RAMDISK_HELPER_H

struct device;

#define RAMDISK_SECTOR_SIZE 512
#define RAMDISK_TOTAL_SECTORS 1000

void ramdisk_helper_remove_rd(struct device* rd);
struct device* ramdisk_helper_create_rd();
#endif