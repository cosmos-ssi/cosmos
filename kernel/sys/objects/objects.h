/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

/*
 * Definition of object types, which are system objects (data spaces, non-system
 * network connections, processes, other users, etc) that the user sees.
 */

#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <types.h>

typedef uint64_t object_handle_t;

typedef object_handle_t obj_executable_t;

#endif