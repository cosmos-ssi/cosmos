//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <object/block/block_object.hpp>

uint32_t BlockObject::readSectors(uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}
uint32_t BlockObject::writeSectors(uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}

uint16_t BlockObject::sectorSize() {
    return 0;
}

uint32_t BlockObject::totalSize() {
    return 0;
}
