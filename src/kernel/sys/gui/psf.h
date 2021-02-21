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
#ifndef _PSF_H
#define _PSF_H

#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

struct psf_font {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
};

struct psf_font* psf_new();
void psf_load(struct psf_font* font, uint8_t* devname, uint8_t* filename);
void psf_delete(struct psf_font* font);

#endif
