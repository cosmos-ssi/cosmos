/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <subsystems.h>
#include <types.h>

bool driver_compare_id(uint64_t id1[4], uint64_t id2[4]) {
    if ((id1[0] == id2[0]) && (id1[1] == id2[1]) && (id1[2] == id2[2]) && (id1[3] == id2[3])) {
        return true;
    } else {
        return false;
    }
}