/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _INTERFACES_ENUMERABLE_H
#define _INTERFACES_ENUMERABLE_H

typedef struct interface_enumerable_t {
    void* (*enumerate)();
} interface_enumerable_t;

#endif