/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DTABLE_H
#define _DTABLE_H

#include <types.h>

#define DTABLE_LEVEL_1(idx) (idx >> 56)
#define DTABLE_LEVEL_2(idx) ((idx & 0x00FF000000000000) >> 48)
#define DTABLE_LEVEL_3(idx) ((idx & 0x0000FF0000000000) >> 40)
#define DTABLE_LEVEL_4(idx) ((idx & 0x000000FF00000000) >> 32)
#define DTABLE_LEVEL_5(idx) ((idx & 0x00000000FF000000) >> 24)
#define DTABLE_LEVEL_6(idx) ((idx & 0x0000000000FF0000) >> 16)
#define DTABLE_LEVEL_7(idx) ((idx & 0x000000000000FF00) >> 8)
#define DTABLE_LEVEL_8(idx) (idx & 0x00000000000000FF)

typedef void********* dtable;

//void* dtable_get(dtable table, uint64_t idx);

#endif