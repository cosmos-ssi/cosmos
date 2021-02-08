/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/proc/proc.h>
#include <sys/string/mem.h>

ptable_t ptable;

bool add_proc_entry(proc_info_t* proc) {
    /*
     * Returns false if unable to allocate needed memory for any reason.  Can
     * also be used to modify an existing entry
     */
    pid_t pid;

    pid = proc->pid;

    // apologies for the mess that is about to unfold
    if (!ptable) {

        ptable = (proc_info_t*********)kmalloc(sizeof(proc_info_t*********) * 256);

        if (!ptable) {
            return false;
        }

        memset((uint8_t*)ptable, 0, sizeof(proc_info_t*********) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)] = (proc_info_t********)kmalloc(sizeof(proc_info_t********) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)], 0, sizeof(proc_info_t********) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)] =
            (proc_info_t*******)kmalloc(sizeof(proc_info_t*******) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)], 0, sizeof(proc_info_t*******) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)] =
            (proc_info_t******)kmalloc(sizeof(proc_info_t******) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)], 0,
               sizeof(proc_info_t******) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)] =
            (proc_info_t*****)kmalloc(sizeof(proc_info_t*****) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)], 0,
               sizeof(proc_info_t*****) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
               [PTABLE_LEVEL_5(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
              [PTABLE_LEVEL_5(pid)] = (proc_info_t****)kmalloc(sizeof(proc_info_t****) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
                               [PTABLE_LEVEL_5(pid)],
               0, sizeof(proc_info_t****) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
               [PTABLE_LEVEL_6(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
              [PTABLE_LEVEL_6(pid)] = (proc_info_t***)kmalloc(sizeof(proc_info_t***) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
                               [PTABLE_LEVEL_5(pid)][PTABLE_LEVEL_6(pid)],
               0, sizeof(proc_info_t***) * 256);
    }

    if (!ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
               [PTABLE_LEVEL_6(pid)][PTABLE_LEVEL_7(pid)]) {

        ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
              [PTABLE_LEVEL_6(pid)][PTABLE_LEVEL_7(pid)] = (proc_info_t**)kmalloc(sizeof(proc_info_t**) * 256);

        memset((uint8_t*)ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)]
                               [PTABLE_LEVEL_5(pid)][PTABLE_LEVEL_6(pid)][PTABLE_LEVEL_7(pid)],
               0, sizeof(proc_info_t**) * 256);
    }

    ptable[PTABLE_LEVEL_1(pid)][PTABLE_LEVEL_2(pid)][PTABLE_LEVEL_3(pid)][PTABLE_LEVEL_4(pid)][PTABLE_LEVEL_5(pid)]
          [PTABLE_LEVEL_6(pid)][PTABLE_LEVEL_7(pid)][PTABLE_LEVEL_8(pid)] = proc;

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