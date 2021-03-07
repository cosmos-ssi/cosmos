//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* every cosmos host in the cluster needs an id.  this object provides that id
*/
#ifndef _HOSTID_H
#define _HOSTID_H

#include <types.h>

struct object;

struct object* hostid_attach();
void hostid_detach(struct object* obj);

#endif