/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/proc/proc.h>

ptable_t ptable;

proc_info_t* get_proc_info(pid_t pid) {
    /*
     * Walk the levels of ptable, if null at any level then the process doesn't
     * exist so return NULL.  Otherwise return pointer to proc_info struct.
     */

    // sorry about this...
    if (!ptable) {
        return NULL;
    }

    if (!ptable[pid >> 56]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]
               [(pid & 0x000000FF00000000) >> 32]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]
               [(pid & 0x000000FF00000000) >> 32][(pid & 0x00000000FF000000) >> 24]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]
               [(pid & 0x000000FF00000000) >> 32][(pid & 0x00000000FF000000) >> 24][(pid & 0x0000000000FF0000) >> 16]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]
               [(pid & 0x000000FF00000000) >> 32][(pid & 0x00000000FF000000) >> 24][(pid & 0x0000000000FF0000) >> 16]
               [(pid & 0x000000000000FF00) >> 8]) {
        return NULL;
    }

    if (!ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]
               [(pid & 0x000000FF00000000) >> 32][(pid & 0x00000000FF000000) >> 24][(pid & 0x0000000000FF0000) >> 16]
               [(pid & 0x000000000000FF00) >> 8][pid & 0x00000000000000FF]) {
        return NULL;
    }

    return ptable[pid >> 56][(pid & 0x00FF000000000000) >> 48][(pid & 0x0000FF0000000000) >> 40]
                 [(pid & 0x000000FF00000000) >> 32][(pid & 0x00000000FF000000) >> 24][(pid & 0x0000000000FF0000) >> 16]
                 [(pid & 0x000000000000FF00) >> 8][pid & 0x00000000000000FF];
}