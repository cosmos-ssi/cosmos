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

    static uint64_t strlen(const uint8_t* s);
    static uint8_t* strtrim(const uint8_t* s);
    static uint8_t* strncpy(uint8_t* dest, const uint8_t* src, uint64_t len);
    static uint8_t* strncat(uint8_t* dest, const uint8_t* src, uint64_t len);
    static uint8_t strcmp(const uint8_t* str1, const uint8_t* str2);
    static uint8_t strncmp(const uint8_t* str1, const uint8_t* str2, uint64_t len);
    static uint32_t strstr(const uint8_t* str1, uint32_t start, const uint8_t* str2);
    static uint8_t* substr(const uint8_t* str1, uint32_t start, uint32_t end, uint8_t* str2, uint32_t size);

  private:
    uint64_t sz;
    uint8_t* m_data;
};

#endif