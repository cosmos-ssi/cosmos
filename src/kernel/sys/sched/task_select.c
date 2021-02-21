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
     * current_task pointer.  It does, however, increment times_skipped as
     * appropriate.
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

            // Since we're now skipping the prior best_candidate...
            TASK_DATA(best_candidate)->times_skipped++;

            // And we have a new best_candidate
            best_candidate = pos;

        } else {
            TASK_DATA(pos)->times_skipped++;
        }

        pos = pos->next;
    } while (pos != current_task[CUR_CPU][CUR_CORE]);

    return best_candidate;
}