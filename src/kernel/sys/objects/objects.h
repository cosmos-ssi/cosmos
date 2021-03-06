/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

/*
 * Definition of object types, which are system objects (data spaces, non-system
 * network connections, processes, other users, etc) that the user sees.
 */

#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <sys/collection/dtable/dtable.h>
#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <types.h>

// given a handle, retrieve object from table and return its data member
#define OBJECT_DATA(handle, type) ((type*)(object_table_get(handle)->data))

typedef enum object_types_t {
    OBJECT_EXECUTABLE,
    OBJECT_PRESENTATION,
    OBJECT_KERNEL_WORK,
    OBJECT_PROCESS,
    OBJECT_TASK
} object_types_t;

typedef uint64_t object_handle_t;

typedef object_handle_t obj_executable_t;

typedef struct object_t {
    object_types_t type;

    /* 
     * This is an optional user-supplied name, independent of any name that may
     * be attached to the object's underlying representation
     */
    char* name;

    void* data;

    object_handle_t handle;
} object_t;

typedef struct object_executable_t {
    uint64_t page_base;
    uint64_t page_count;
    bool from_presentation;  // if false, the value in presentation is not valid
    object_handle_t presentation;
    char* exe_name;
} object_executable_t;

typedef struct object_kernel_work_t {
    void* (*work_func)(void*);
    void* arg;
} object_kernel_work_t;

typedef struct object_presentation_t {
    /*
     * For now just encapsulate enough to use the initrd driver directly, once
     * voh develops switch to that
     */
    filesystem_node_t* node;
} object_presentation_t;

#include <sys/proc/proc.h>

typedef struct object_process_t {
    pid_t pid;
    object_handle_t body;
} object_process_t;

typedef struct object_task_t {
    object_handle_t process;
    linkedlist* sched_task;
} object_task_t;

extern dtable object_table;
extern uint64_t object_table_next_idx;

// object.c
object_handle_t object_create(object_types_t type, void* object_data);
object_types_t object_type_(object_handle_t obj);

// object_executable.c
object_handle_t object_executable_create_from_presentation(object_handle_t pres);

// object_init.c
void object_init();

// object_kernel_work.c
object_handle_t object_kernel_work_create(void* (*work_func)(void*), void* arg);

// object_presentation.c
object_handle_t object_presentation_create(filesystem_node_t* node);

// object_process.c
object_handle_t object_process_create(object_handle_t exe);

// object_table.c
object_t* object_table_get(object_handle_t handle);

// object_task.c
object_handle_t object_task_create(object_handle_t proc);

#endif