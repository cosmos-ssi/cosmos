/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/objects/objects.h>

void object_init() {
    object_table = dtable_init();

    object_table_next_idx = 1;

    return;
}