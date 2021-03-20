//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/fs/file_util.h>
#include <sys/gui/props.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct props* props_load(uint8_t* fs, uint8_t* fn) {
    ASSERT_NOT_NULL(fs);
    ASSERT_NOT_NULL(fn);
    struct props* ret = (struct props*)kmalloc(sizeof(struct props));
    memzero((uint8_t*)ret, sizeof(struct props));
    ret->props = arraylist_new();
    uint32_t size = 0;
    uint8_t* buffer = file_util_read_file(fs, fn, &size);
    ASSERT_NOT_NULL(size);
    ASSERT_NOT_NULL(buffer);

    //  kprintf("size %llu\n", size);
    uint64_t idx = 0;
    while (idx < size) {
        //    kprintf("idx: %llu\n", idx);

        uint64_t cr = strstr(buffer, idx, "\n");
        if (-1 != cr) {
            uint64_t e = strstr(buffer, idx, "=");
            ASSERT(e != -1);
            ASSERT(e < cr);
            struct prop* p = (struct prop*)kmalloc(sizeof(struct prop));
            memcpy(p->key, &buffer[idx], e - idx);
            p->key[e - idx] = 0;
            memcpy(p->value, &buffer[e + 1], cr - (e + 1));
            p->value[cr - (e + 1)] = 0;
            //      kprintf("adding '%s' '%s'\n", p->key, p->value);
            arraylist_add(ret->props, p);
            idx = cr + 1;
        } else {
            idx += 1;
        }
    }
    return ret;
}

uint8_t* props_get(struct props* props, uint8_t* name) {
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(props);
    ASSERT_NOT_NULL(props->props);

    for (uint64_t i = 0; i < arraylist_count(props->props); i++) {
        struct prop* p = (struct prop*)arraylist_get(props->props, i);
        if (0 == strcmp(p->key, name)) {
            return p->value;
        }
    }
    return 0;
}

void props_delete(struct props* props) {
    ASSERT_NOT_NULL(props);
    ASSERT_NOT_NULL(props->props);
    for (uint64_t i = 0; i < arraylist_count(props->props); i++) {
        struct prop* p = (struct prop*)arraylist_get(props->props, i);
        kfree(p);
    }
    kfree(props);
}
