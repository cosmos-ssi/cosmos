/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
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
    linkedlist* idle;

    pos = current_task[CUR_CPU][CUR_CORE];
    best_candidate = pos;

    do {
        ASSERT_NOT_NULL(pos);
        ASSERT_NOT_NULL(best_candidate);

        // Make a note of the idle process in case there are no schedulable processes
        if (TASK_DATA(pos)->pid == 0) {
            idle = pos;
        }

        // Replace the current best_candidate if (a) pos has been skipped more
        // than the current best_candidate, or (b) the current best_candidate is
        // not schedulable (this can only happen if the best_candidate is the
        // current process).
        if ((TASK_DATA(pos)->state == SCHED_SLEEPING) &&
            ((TASK_DATA(pos)->times_skipped > TASK_DATA(best_candidate)->times_skipped) ||
             (TASK_DATA(best_candidate)->state != SCHED_SLEEPING))) {

            // Since we're now skipping the prior best_candidate...
            TASK_DATA(best_candidate)->times_skipped++;

            // And we have a new best_candidate
            best_candidate = pos;

        } else {
            TASK_DATA(pos)->times_skipped++;
        }

        pos = pos->next;
    } while (pos != current_task[CUR_CPU][CUR_CORE]);

    // If there are no schedulable tasks, then best_candidate will still be the
    // current process.  So we have to make sure it's schedulable; if not, we
    // return the idle process.
    if ((TASK_DATA(best_candidate)->state != SCHED_SLEEPING) ||
        (TASK_DATA(best_candidate)->state != SCHED_LASTRESORT)) {
        return idle;
    }

    return best_candidate;
}