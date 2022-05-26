/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#if 0

#include <obj/x86-64/cmos/cmos.h>
#include <obj/x86-64/rtc/rtc.h>
#include <sys/asm/io.h>
#include <sys/asm/misc.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_rtc.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/timing/timerapi.h>
#include <sys/x86-64/idt/irq.h>
#include <types.h>

#define RTC_IRQ_NUMBER 8

// https://wiki.osdev.org/RTC

struct arraylist* rtcEvents;

typedef enum rtc_registers {
    RTC_REGISTER_SECOND = 0x00,
    RTC_REGISTER_MINUTE = 0x02,
    RTC_REGISTER_HOUR = 0x04,
    RTC_REGISTER_WEEKDAY = 0x06,
    RTC_REGISTER_MONTHDAY = 0x07,
    RTC_REGISTER_MONTH = 0x08,
    RTC_REGISTER_YEAR = 0x09,
    RTC_REGISTER_STATUS_A = 0x0A,
    RTC_REGISTER_STATUS_B = 0x0B,
    RTC_REGISTER_STATUS_C = 0x0C,
    RTC_REGISTER_CENTURY = 0x32
} rtc_registers;

rtc_time_t rtc_time(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    rtc_time_t a, b;

    a.second = cmos_read_register(RTC_REGISTER_SECOND);
    a.minute = cmos_read_register(RTC_REGISTER_MINUTE);
    a.hour = cmos_read_register(RTC_REGISTER_HOUR);
    a.weekday = cmos_read_register(RTC_REGISTER_WEEKDAY);
    a.monthday = cmos_read_register(RTC_REGISTER_MONTHDAY);
    a.month = cmos_read_register(RTC_REGISTER_MONTH);
    a.year = cmos_read_register(RTC_REGISTER_YEAR);
    a.century = cmos_read_register(RTC_REGISTER_CENTURY);

    b.second = cmos_read_register(RTC_REGISTER_SECOND);
    b.minute = cmos_read_register(RTC_REGISTER_MINUTE);
    b.hour = cmos_read_register(RTC_REGISTER_HOUR);
    b.weekday = cmos_read_register(RTC_REGISTER_WEEKDAY);
    b.monthday = cmos_read_register(RTC_REGISTER_MONTHDAY);
    b.month = cmos_read_register(RTC_REGISTER_MONTH);
    b.year = cmos_read_register(RTC_REGISTER_YEAR);
    b.century = cmos_read_register(RTC_REGISTER_CENTURY);

    if ((a.second == b.second) && (a.minute == b.minute) && (a.hour == b.hour) && (a.weekday == b.weekday) &&
        (a.monthday == b.monthday) && (a.month == b.month) && (a.year == b.year) && (a.century == b.century)) {
        return b;
    }

    do {
        a = b;

        b.second = cmos_read_register(RTC_REGISTER_SECOND);
        b.minute = cmos_read_register(RTC_REGISTER_MINUTE);
        b.hour = cmos_read_register(RTC_REGISTER_HOUR);
        b.weekday = cmos_read_register(RTC_REGISTER_WEEKDAY);
        b.monthday = cmos_read_register(RTC_REGISTER_MONTHDAY);
        b.month = cmos_read_register(RTC_REGISTER_MONTH);
        b.year = cmos_read_register(RTC_REGISTER_YEAR);
        b.century = cmos_read_register(RTC_REGISTER_CENTURY);
    } while ((a.second != b.second) || (a.minute != b.minute) || (a.hour != b.hour) || (a.weekday != b.weekday) ||
             (a.monthday != b.monthday) || (a.month != b.month) || (a.year != b.year) || (a.century != b.century));

    return b;
}

#endif