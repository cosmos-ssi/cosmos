/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <types.h>

void* dtable_get(dtable table, uint64_t idx) {
    /*
     * Walk the levels of ptable, if null at any level then the process doesn't
     * exist so return NULL.  Otherwise return pointer to proc_info struct.
     */

    // sorry about this...
    if (!table) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)]) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)]) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)]) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
              [DTABLE_LEVEL_5(idx)]) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
              [DTABLE_LEVEL_6(idx)]) {
        return NULL;
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
              [DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)]) {
        return NULL;
    }

    return table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                [DTABLE_LEVEL_5(idx)][DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)][DTABLE_LEVEL_8(idx)];
}

bool dtable_set(dtable table, uint64_t idx, void* data) {
    /*
     * Returns false if unable to allocate needed memory for any reason.  Can
     * also be used to modify an existing entry
     */

    // apologies for the mess that is about to unfold

    if (!table[DTABLE_LEVEL_1(idx)]) {

        table[DTABLE_LEVEL_1(idx)] = (void********)kmalloc(sizeof(void********) * 256);

        if (!table[DTABLE_LEVEL_1(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)], 0, sizeof(void********) * 256);
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)]) {

        table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)] = (void*******)kmalloc(sizeof(void*******) * 256);

        if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)], 0, sizeof(void*******) * 256);
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)]) {

        table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)] =
            (void******)kmalloc(sizeof(void******) * 256);

        if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)], 0,
               sizeof(void******) * 256);
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]) {

        table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)] =
            (void*****)kmalloc(sizeof(void*****) * 256);

        if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)], 0,
               sizeof(void*****) * 256);
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
              [DTABLE_LEVEL_5(idx)]) {

        table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)] =
            (void****)kmalloc(sizeof(void****) * 256);

        if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                  [DTABLE_LEVEL_5(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                              [DTABLE_LEVEL_5(idx)],
               0, sizeof(void****) * 256);
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
              [DTABLE_LEVEL_6(idx)]) {

        table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
             [DTABLE_LEVEL_6(idx)] = (void***)kmalloc(sizeof(void***) * 256);

        if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                  [DTABLE_LEVEL_5(idx)][DTABLE_LEVEL_6(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                              [DTABLE_LEVEL_5(idx)][DTABLE_LEVEL_6(idx)],
               0, sizeof(void***) * 256);
    }

    if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
              [DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)]) {

        table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
             [DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)] = (void**)kmalloc(sizeof(void**) * 256);

        if (!table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                  [DTABLE_LEVEL_5(idx)][DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)]) {
            return false;
        }

        memset((uint8_t*)table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)]
                              [DTABLE_LEVEL_5(idx)][DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)],
               0, sizeof(void**) * 256);
    }

    table[DTABLE_LEVEL_1(idx)][DTABLE_LEVEL_2(idx)][DTABLE_LEVEL_3(idx)][DTABLE_LEVEL_4(idx)][DTABLE_LEVEL_5(idx)]
         [DTABLE_LEVEL_6(idx)][DTABLE_LEVEL_7(idx)][DTABLE_LEVEL_8(idx)] = data;

    return true;
}

dtable dtable_init() {
    return (dtable)kmalloc(sizeof(dtable));
}