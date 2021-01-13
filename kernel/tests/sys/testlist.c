//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/sys/testlist.h>
#include <sys/debug/debug.h>
#include <sys/collection/list/list.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/string/string.h>

char C_A[] = {"A"};
char C_B[] = {"BB"};
char C_C[] = {"CCC"};
char C_D[] = {"DDDD"};
char C_E[] = {"EEEEE"};
char C_F[] = {"FFFFFF"};

void test_list() {
    struct list* lst = list_new();
    list_add(lst, C_A);
    ASSERT(0==strcmp(list_get(lst,0),C_A));
    ASSERT(1==list_count(lst));
    list_add(lst, C_B);
    list_add(lst, C_C);
    list_add(lst, C_D);
    list_add(lst, C_E);
    list_add(lst, C_F);
    ASSERT(6==list_count(lst));
    kprintf("list count %llu\n", list_count(lst));
    for (uint32_t i=0;i<list_count(lst);i++) {
        kprintf("i %llu %s\n",i, list_get(lst,i));
    }

    list_remove(lst, 2);  // letter C
    ASSERT(5==list_count(lst));
    for (uint32_t i=0;i<list_count(lst);i++) {
        kprintf("i %llu %s\n",i, list_get(lst,i));
    }
    ASSERT(0==strcmp(list_get(lst,0),C_A));

    list_remove(lst, 4);  // last element, special case
    for (uint32_t i=0;i<list_count(lst);i++) {
        kprintf("i %llu %s\n",i, list_get(lst,i));
    }
}