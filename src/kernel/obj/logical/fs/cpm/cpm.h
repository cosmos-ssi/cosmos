//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CPM_H
#define _CPM_H

struct device;

struct object* cpm_attach(struct object* partition_device);
void cpm_detach(struct object* dev);

#endif