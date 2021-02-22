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

#define PSF_FONT_MAGIC 0x3604

#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

struct psf1_font {
    uint8_t magic[2];
    uint8_t filemode;
    uint8_t fontheight;
};

struct psf1_font* psf_load(uint8_t* devname, uint8_t* filename);
void psf_delete(struct psf1_font* font);
void psf_dump(struct psf1_font* font);

#endif
