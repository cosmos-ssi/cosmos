//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}
#include <assert.hpp>
#include <rgb.hpp>

uint32_t rgb_components::rgb_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (b << 8) | g;
}

uint8_t rgb_components::rgb_r(uint32_t rgb) {
    return (rgb & 0x00FF0000) >> 16;
}
uint8_t rgb_components::rgb_g(uint32_t rgb) {
    return (rgb & 0x0000FF00) >> 8;
}
uint8_t rgb_components::rgb_b(uint32_t rgb) {
    return (rgb & 0x000000FF);
}

rgb_components::rgb_components(uint32_t rgb) {
    this->b = (rgb & 0x000000FF);
    this->g = (rgb & 0x0000FF00) >> 8;
    this->r = (rgb & 0x00FF0000) >> 16;
}
