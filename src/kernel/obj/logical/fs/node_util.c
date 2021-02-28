//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/node_util.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct filesystem_node* filesystem_node_new(enum filesystem_node_type type, struct object* dev, const uint8_t* name,
                                            uint64_t id, void* node_data) {

    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(name);
    struct filesystem_node* ret = (struct filesystem_node*)kmalloc(sizeof(struct filesystem_node));
    memzero((uint8_t*)ret, sizeof(struct filesystem_node));
    ret->type = type;
    ret->filesystem_device = dev;
    strncpy(ret->name, name, FILESYSTEM_MAX_NAME);
    ret->id = id;
    ret->node_data = node_data;
    return ret;
}
