//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RTC_OBJECT_HPP
#define _RTC_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

struct rtc_time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t weekday;
    uint8_t monthday;
    uint8_t month;
    uint8_t year;
    uint8_t century;
};

class RTCObject : public Object {
  public:
    void read(struct rtc_time* time);
};

#endif