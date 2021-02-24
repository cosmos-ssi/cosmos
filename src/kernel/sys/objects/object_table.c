/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

// Right now these are basically just wrappers for the dtable functions, but
// using them instead of dtables directly should simplify matters if we ever
// change the underlying representation of the table

#include <sys/objects/objects.h>

dtable object_table;
uint64_t object_table_next_idx;

object_t* object_table_get(object_handle_t handle) {
    return (object_t*)dtable_get(object_table, (uint64_t)handle);
}