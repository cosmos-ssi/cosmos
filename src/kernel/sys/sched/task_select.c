/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/sched/sched.h>

linkedlist* task_select() {
    /*
     * This function does not actually start the next task or update the
     * current_task pointer, because sometimes it may be useful to know what the
     * next task to be switched to would be, without actually switching to it
     */

    linkedlist* pos;
    linkedlist* best_candidate;

    pos = current_task[CUR_CPU][CUR_CORE];
    best_candidate = pos;

    do {
        ASSERT_NOT_NULL(pos);
        ASSERT_NOT_NULL(best_candidate);

        if ((TASK_DATA(pos)->times_skipped > TASK_DATA(best_candidate)->times_skipped) &&
            (TASK_DATA(pos)->state == SCHED_SLEEPING)) {
            best_candidate = pos;
        }

        pos = pos->next;
    } while (pos != current_task[CUR_CPU][CUR_CORE]);

    return best_candidate;
}