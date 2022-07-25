/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _INTERfACES_GENERIC_H
#define _INTERFACES_GENERIC_H

#include <api/interfaces/interfaces.h>

typedef struct interface_generic_t {
    void* (*constructor)();
    void* (*initialize)(void*);
    implemented_interfaces_list* (*interfaces)();
} interface_generic_t;

#endif