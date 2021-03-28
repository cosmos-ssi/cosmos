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
#include <object/objects.hpp>
#include <tga.hpp>
/**
 * Parse TGA format into pixels. Returns 0 or error, otherwise the returned data looks like
 *   ret[0] = width of the image
 *   ret[1] = height of the image
 *   ret[2..] = 32 bit ARGB pixels (blue channel in the least significant byte, alpha channel in the most)
 */
uint32_t* tga::parse(uint8_t* ptr, int size) {
    uint32_t* data;
    int i, j, k, x, y, w = (ptr[13] << 8) + ptr[12], h = (ptr[15] << 8) + ptr[14], o = (ptr[11] << 8) + ptr[10];
    int m = ((ptr[1] ? (ptr[7] >> 3) * ptr[5] : 0) + 18);

    if (w < 1 || h < 1)
        return 0;

    data = (uint32_t*)malloc((w * h + 2) * sizeof(uint32_t));
    if (!data)
        return 0;

    switch (ptr[2]) {
        case 1:
            if (ptr[6] != 0 || ptr[4] != 0 || ptr[3] != 0 || (ptr[7] != 24 && ptr[7] != 32)) {
                free(data);
                return 0;
            }
            for (y = i = 0; y < h; y++) {
                k = ((!o ? h - y - 1 : y) * w);
                for (x = 0; x < w; x++) {
                    j = ptr[m + k++] * (ptr[7] >> 3) + 18;
                    data[2 + i++] =
                        ((ptr[7] == 32 ? ptr[j + 3] : 0xFF) << 24) | (ptr[j + 2] << 16) | (ptr[j + 1] << 8) | ptr[j];
                }
            }
            break;
        case 2:
            if (ptr[5] != 0 || ptr[6] != 0 || ptr[1] != 0 || (ptr[16] != 24 && ptr[16] != 32)) {
                free(data);
                return 0;
            }
            for (y = i = 0; y < h; y++) {
                j = ((!o ? h - y - 1 : y) * w * (ptr[16] >> 3));
                for (x = 0; x < w; x++) {
                    data[2 + i++] =
                        ((ptr[16] == 32 ? ptr[j + 3] : 0xFF) << 24) | (ptr[j + 2] << 16) | (ptr[j + 1] << 8) | ptr[j];
                    j += ptr[16] >> 3;
                }
            }
            break;
        case 9:
            if (ptr[6] != 0 || ptr[4] != 0 || ptr[3] != 0 || (ptr[7] != 24 && ptr[7] != 32)) {
                free(data);
                return 0;
            }
            y = i = 0;
            for (x = 0; x < w * h && m < size;) {
                k = ptr[m++];
                if (k > 127) {
                    k -= 127;
                    x += k;
                    j = ptr[m++] * (ptr[7] >> 3) + 18;
                    while (k--) {
                        if (!(i % w)) {
                            i = ((!o ? h - y - 1 : y) * w);
                            y++;
                        }
                        data[2 + i++] = ((ptr[7] == 32 ? ptr[j + 3] : 0xFF) << 24) | (ptr[j + 2] << 16) |
                                        (ptr[j + 1] << 8) | ptr[j];
                    }
                } else {
                    k++;
                    x += k;
                    while (k--) {
                        j = ptr[m++] * (ptr[7] >> 3) + 18;
                        if (!(i % w)) {
                            i = ((!o ? h - y - 1 : y) * w);
                            y++;
                        }
                        data[2 + i++] = ((ptr[7] == 32 ? ptr[j + 3] : 0xFF) << 24) | (ptr[j + 2] << 16) |
                                        (ptr[j + 1] << 8) | ptr[j];
                    }
                }
            }
            break;
        case 10:
            if (ptr[5] != 0 || ptr[6] != 0 || ptr[1] != 0 || (ptr[16] != 24 && ptr[16] != 32)) {
                free(data);
                return 0;
            }
            y = i = 0;
            for (x = 0; x < w * h && m < size;) {
                k = ptr[m++];
                if (k > 127) {
                    k -= 127;
                    x += k;
                    while (k--) {
                        if (!(i % w)) {
                            i = ((!o ? h - y - 1 : y) * w);
                            y++;
                        }
                        data[2 + i++] = ((ptr[16] == 32 ? ptr[m + 3] : 0xFF) << 24) | (ptr[m + 2] << 16) |
                                        (ptr[m + 1] << 8) | ptr[m];
                    }
                    m += ptr[16] >> 3;
                } else {
                    k++;
                    x += k;
                    while (k--) {
                        if (!(i % w)) {
                            i = ((!o ? h - y - 1 : y) * w);
                            y++;
                        }
                        data[2 + i++] = ((ptr[16] == 32 ? ptr[m + 3] : 0xFF) << 24) | (ptr[m + 2] << 16) |
                                        (ptr[m + 1] << 8) | ptr[m];
                        m += ptr[16] >> 3;
                    }
                }
            }
            break;
        default:
            free(data);
            return 0;
    }
    data[0] = w;
    data[1] = h;
    return data;
}

tga::tga(uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);

    uint32_t len;
    uint8_t* data = FilesystemObject::read_file(devname, filename, &len);
    //  kprintf("bmp size %llu\n", len);
    this->buffer = data;
    this->header = (struct tga_header*)data;
    this->data = (uint32_t*)&(data[sizeof(struct tga_header)]);
    //  kprintf("bpp %llu\n", ret->header->bpp);
    //  kprintf("encoding %llu\n", ret->header->encoding);

    ASSERT(this->header->colormap == 0);
    ASSERT(this->header->encoding == 2);
    ASSERT(this->header->bpp == 32);
    ASSERT(this->header->pixeltype == 40);
}

tga::~tga() {
    if (0 != this->buffer) {
        free(this->buffer);
    }
}

void tga::tga_dump() {
    //  kprintf("w %llu h %llu x %llu y %llu\n", targa->header->w, targa->header->h, targa->header->x, targa->header->y);
    //  kprintf("encoding %llu\n", targa->header->encoding);
    //  kprintf("bpp %llu\n", targa->header->bpp);
}
