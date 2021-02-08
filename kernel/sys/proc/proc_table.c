/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/proc/proc.h>

ptable_t ptable;

bool add_proc_entry(proc_info_t proc) {
    if (!ptable) {
        ptable = (ptable_t)kmalloc(sizeof(proc_info_t********) * 256);
    }

    return true;
}

proc_info_t* get_proc_info(pid_t pid) {
    /*
     * Walk the levels of ptable, if null at any level then the process doesn't
     * exist so return NULL.  Otherwise return pointer to proc_info struct.
     */

    // sorry about this...
    if (!ptable) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)]) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)]) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)]) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
               [PTABLE_LEVEL_5(pid)]) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
               [PTABLE_LEVEL_6(pid)]) {
        return NULL;
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
               [PTABLE_LEVEL_6(pid)][PTABLE_LEVEL_7(pid)]) {
        return NULL;
    }

    return ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
                 [PTABLE_LEVEL_5(pid)][PTABLE_LEVEL_6(pid)][PTABLE_LEVEL_7(pid)][PTABLE_LEVEL_8(pid)];
}