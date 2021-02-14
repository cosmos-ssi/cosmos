//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TFS_H
#define _TFS_H

struct device;

struct device* tfs_attach(struct device* partition_device);
void tfs_detach(struct device* dev);

#endif