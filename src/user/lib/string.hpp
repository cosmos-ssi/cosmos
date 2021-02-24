//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _STRING_HPP
#define _STRING_HPP

#include <types.h>

class String {
  public:
    String(const uint8_t* ptr);
    String();
    String(const String& obj);
    String(String&& obj);
    ~String();
    const uint64_t getSize();
    const uint8_t* getData();

  private:
    uint64_t sz;
    uint8_t* m_data;
};

#endif