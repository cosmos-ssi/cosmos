//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all time objects will implement
 */
#ifndef _OBJECTTYPE_TIME_H
#define _OBJECTTYPE_TIME_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef struct time_time_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t weekday;
    uint8_t monthday;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} time_time_t;

typedef time_time_t (*time_time_function)(struct object* obj);

struct objectinterface_time {
    time_time_function time;
};

#endif