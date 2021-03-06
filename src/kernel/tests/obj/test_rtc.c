//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_rtc.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_rtc.h>

void test_rtc() {
    // get the time, b/c we can
    struct object* rtc = objectmgr_find_object_by_name("rtc0");
    if (0 != rtc) {
        struct objectinterface_rtc* rtc_api = (struct objectinterface_rtc*)rtc->api;
        rtc_time_function time_func = rtc_api->rtc_time;
        rtc_time_t daTime = (*time_func)(rtc);
        kprintf("Hour: %llu Minute: %llu Second: %llu\n", daTime.hour, daTime.minute, daTime.second);
    } else {
        kprintf("Unable to find rtc0\n");
    }
}