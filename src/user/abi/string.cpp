//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/str.h>
#include <abi/string.hpp>

String::String(const uint8_t* ptr) {
    this->sz = strlen(ptr) + 1;
    this->m_data = ptr;
}

String::String() : m_data(nullptr), sz(0) {}

String::~String() {
    delete[] m_data;
}

String::String(const String& obj) {
    this->sz = obj.sz;
    this->m_data = new uint8_t[sz];
    strncpy(this->m_data, obj.m_data, sz);
}

String::String(String&& obj) noexcept : sz(obj.sz), m_data(obj.m_data) {
    obj.sz = 0;
    obj.m_data = 0;
}

const uint64_t String::getSize() {
    return sz;
}

const uint8_t* String::getData() {
    return m_data;
}
