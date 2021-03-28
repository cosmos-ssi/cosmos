//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* PSF font
*/
// https://wiki.osdev.org/PC_Screen_Font
// https://iot-programmer.com/index.php/articles/43-c/103-framebuffer-text-using-psf-1-fonts

#ifndef _PSF_HPP
#define _PSF_HPP

#include <types.h>

struct psf1_font_data {
    uint8_t magic[2];
    uint8_t filemode;
    uint8_t fontheight;
};

class psf1_font {
  private:
    struct psf1_font_data* data;

  public:
    psf1_font(uint8_t* devname, uint8_t* filename);
    ~psf1_font();
    void dump();
    uint32_t total_characters();
    uint32_t height();
    uint8_t is_unicode();
    uint8_t* character(uint16_t idx);
};

#endif
