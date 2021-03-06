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

#ifndef _PSF_H
#define _PSF_H

#include <types.h>

struct psf1_font {
    uint8_t magic[2];
    uint8_t filemode;
    uint8_t fontheight;
};

struct psf1_font* psf_load(uint8_t* devname, uint8_t* filename);
void psf_delete(struct psf1_font* font);
void psf_dump(struct psf1_font* font);
uint32_t psf_total_characters(struct psf1_font* font);
uint32_t psf_height(struct psf1_font* font);
uint8_t psf_is_unicode(struct psf1_font* font);
uint8_t* psf_character(struct psf1_font* font, uint16_t idx);
#endif
