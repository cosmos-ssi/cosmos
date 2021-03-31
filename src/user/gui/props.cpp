//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
#include <mem.h>
}

#include <arraylist.hpp>
#include <assert.hpp>
#include <object/objects.hpp>
#include <props.hpp>
#include <string.hpp>

Props::Props() {
    this->props = new ArrayList<Prop>();
}

Props::~Props() {
    for (uint64_t i = 0; i < this->props->getCount(); i++) {
        Prop* p = this->props->get(i);
        delete p;
    }
}

uint8_t* Props::get(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    for (uint64_t i = 0; i < this->props->getCount(); i++) {
        Prop* p = this->props->get(i);
        if (0 == String::strcmp(p->key, name)) {
            return p->value;
        }
    }
    return 0;
}

void Props::load(uint8_t* fs, uint8_t* fn) {
    ASSERT_NOT_NULL(fn);
    uint32_t size = 0;
    uint8_t* buffer = FilesystemObject::read_file(fs, fn, &size);
    ASSERT_NOT_NULL(size);
    ASSERT_NOT_NULL(buffer);

    uint64_t idx = 0;
    while (idx < size) {
        uint64_t cr = String::strstr(buffer, idx, (uint8_t*)"\n");
        if (-1 != cr) {
            uint64_t e = String::strstr(buffer, idx, (uint8_t*)"=");
            ASSERT(e != -1);
            ASSERT(e < cr);
            Prop* p = new Prop();
            memcpy(p->key, &(buffer[idx]), e - idx);
            p->key[e - idx] = 0;
            memcpy(p->value, &(buffer[e + 1]), cr - (e + 1));
            p->value[cr - (e + 1)] = 0;
            this->props->add(p);
            idx = cr + 1;
        } else {
            idx += 1;
        }
    }
}
