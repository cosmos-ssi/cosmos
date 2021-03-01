//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_OBJECT_HPP
#define _BLOCK_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class BlockObject : public Object {
  public:
    uint32_t readSectors(uint8_t* data, uint32_t data_size, uint32_t start_lba);
    uint32_t writeSectors(uint8_t* data, uint32_t data_size, uint32_t start_lba);
    uint16_t sectorSize();
    uint32_t totalSize();
};

#endif