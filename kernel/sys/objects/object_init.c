/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>
#include <sys/string/mem.h>

object_t** object_table;
uint64_t object_table_last_idx, object_table_dim;

void object_init() {
    object_table = (object_t**)kmalloc(sizeof(object_handle_t*) * 64);

    memset((uint8_t*)object_table, 0, sizeof(object_t*) * 64);

    return;
}