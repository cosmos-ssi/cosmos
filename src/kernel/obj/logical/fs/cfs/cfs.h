//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CFS_H
#define _CFS_H

struct object;

struct object* cfs_attach(struct object* partition_object);
void cfs_detach(struct object* obj);

#endif