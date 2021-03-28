//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RGB_HPP
#define _RGB_HPP

#include <types.h>

class rgb_components {
  public:
    uint8_t r;
    uint8_t g;
    uint8_t b;

    static uint32_t rgb_rgb(uint8_t r, uint8_t g, uint8_t b);
    static uint8_t rgb_r(uint32_t rgb);
    static uint8_t rgb_g(uint32_t rgb);
    static uint8_t rgb_b(uint32_t rgb);
    rgb_components(uint32_t rgb);
};

#endif
