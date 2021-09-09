/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SUBSYSTEMS_H
#define _SUBSYSTEMS_H

#include <types.h>

// defines subsystem_list_t enum, for used in automated subsystem/driver
// registration, as well as various linker set macros

// function pointer type for driver initialization functions
typedef void (*driver_initialization_function_t)(void*);

// N.B. that numbering of items in this enum is done automatically, which means
// that modules that reference it will not retain binary-compatibility if the
// ordering of elements is ever changed.
typedef enum subsystem_list_t { SUBSYSTEM_ID_TIMER, SUBSYSTEM_ID_MAX } subsystem_list_t;

typedef struct driver_info_1_t {
    const char* shortname;
    const char* longname;
    const char* description;
    const char* authors;
    const char* contact;
    const char* version;
    uint64_t id[4];  // 256-bit unique value; I suggest using https://www.random.org/bytes/
    subsystem_list_t subsystem;
    driver_initialization_function_t init_func;
} driver_info_1_t;

typedef struct driver_list_entry_t {
    uint64_t driver_interface_version;  // currently at version 1
    void* driver_info;
} driver_list_entry_t;

#define SUBSYSTEM_LIST_START                                                                                           \
    driver_list_entry_t driver_list_start __attribute__((section(".driverlist.tag.start"))) = {0}

#define SUBSYSTEM_LIST_END driver_list_entry_t driver_list_end __attribute__((section(".driverlist.tag.end"))) = {0}

#define SUBSYSTEM_DRIVER(identifier, longname, description, authors, contact, version, id1, id2, id3, id4, subsystem,  \
                         init_func)                                                                                    \
    driver_info_1_t driver_info_##identifier = {                                                                       \
        #identifier, longname, description, authors, contact, version, {id1, id2, id3, id4}, subsystem, init_func};    \
    driver_list_entry_t driver_entry_##identifier                                                                      \
        __attribute__((section(".driverlist.body." #identifier))) = {1, &driver_info_##identifier};

#endif