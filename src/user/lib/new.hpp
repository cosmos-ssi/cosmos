//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file globally overrides C++ new and delete
*/
#ifndef _NEW_HPP
#define _NEW_HPP

#include <types.h>

void* operator new(uint64_t count);
void operator delete(void* ptr);

#endif