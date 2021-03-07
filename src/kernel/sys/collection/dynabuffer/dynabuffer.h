
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DYNABUFFER_H
#define _DYNABUFFER_H

#include <types.h>

struct dynabuffer {
    uint8_t* data;
    uint32_t size;
    uint32_t idx;
};

struct dynabuffer* dynabuffer_new();
void dynabuffer_delete(struct dynabuffer* db);
void dynabuffer_reset(struct dynabuffer* db);
uint32_t dynabuffer_size(struct dynabuffer* db);
uint32_t dynabuffer_idx(struct dynabuffer* db);
uint8_t* dynabuffer_data(struct dynabuffer* db);

void dynabuffer_append_uint8_t(struct dynabuffer* db, uint8_t v);
void dynabuffer_append_uint16_t(struct dynabuffer* db, uint16_t v);
void dynabuffer_append_uint32_t(struct dynabuffer* db, uint32_t v);
void dynabuffer_append_uint64_t(struct dynabuffer* db, uint64_t v);

uint8_t dynabuffer_read_uint8_t(struct dynabuffer* db);
uint16_t dynabuffer_read_uint16_t(struct dynabuffer* db);
uint32_t dynabuffer_read_uint32_t(struct dynabuffer* db);
uint64_t dynabuffer_read_uint64_t(struct dynabuffer* db);

void dynabuffer_append_string(struct dynabuffer* db, uint8_t* str);
void dynabuffer_read_string(struct dynabuffer* db, uint8_t* str, uint32_t size);

#endif
