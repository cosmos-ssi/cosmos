//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CFS_H
#define _CFS_H

struct device;

struct device* cfs_attach(struct device* partition_device);
void cfs_detach(struct device* dev);

#endif