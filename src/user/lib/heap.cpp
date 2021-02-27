//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
    2014 Leonard Kevin McGuire Jr (www.kmcg3413.net) (kmcg3413@gmail.com)
    2016 Clément Gallet (provided bug fixes)
*/

#include <heap.hpp>

Heap::Heap() {
    fblock = 0;
}

int Heap::add(uint64_t addr, uint32_t size, uint32_t bsize) {
    Heap_block_bitmap* b;
    uint32_t bcnt;
    uint32_t x;
    uint8_t* bm;

    b = (Heap_block_bitmap*)addr;
    b->size = size - sizeof(Heap_block_bitmap);
    b->bsize = bsize;

    b->next = fblock;
    fblock = b;

    bcnt = b->size / b->bsize;
    bm = (uint8_t*)&b[1];

    /* clear bitmap */
    for (x = 0; x < bcnt; ++x) {
        bm[x] = 0;
    }

    /* reserve room for bitmap */
    bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
    for (x = 0; x < bcnt; ++x) {
        bm[x] = 5;
    }

    b->lfb = bcnt - 1;

    b->used = bcnt;

    return 1;
}

uint8_t Heap::getNID(uint8_t a, uint8_t b) {
    uint8_t c;
    for (c = a + 1; c == b || c == 0; ++c)
        ;
    return c;
}

void* Heap::alloc(uint32_t size) {
    Heap_block_bitmap* b;
    uint8_t* bm;
    uint32_t bcnt;
    uint32_t x, y, z;
    uint32_t bneed;
    uint8_t nid;

    /* iterate blocks */
    for (b = fblock; b; b = b->next) {
        /* check if block has enough room */
        if (b->size - (b->used * b->bsize) >= size) {

            bcnt = b->size / b->bsize;
            bneed = (size / b->bsize) * b->bsize < size ? size / b->bsize + 1 : size / b->bsize;
            bm = (uint8_t*)&b[1];

            for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x < b->lfb; ++x) {
                /* just wrap around */
                if (x >= bcnt) {
                    x = 0;
                }

                if (bm[x] == 0) {
                    /* count free blocks */
                    for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; ++y)
                        ;

                    /* we have enough, now allocate them */
                    if (y == bneed) {
                        /* find ID that does not match left or right */
                        nid = getNID(bm[x - 1], bm[x + y]);

                        /* allocate by setting id */
                        for (z = 0; z < y; ++z) {
                            bm[x + z] = nid;
                        }

                        /* optimization */
                        b->lfb = (x + bneed) - 2;

                        /* count used blocks NOT bytes */
                        b->used += y;

                        return (void*)(x * b->bsize + (uint64_t)&b[1]);
                    }

                    /* x will be incremented by one ONCE more in our FOR loop */
                    x += (y - 1);
                    continue;
                }
            }
        }
    }

    return 0;
}

void Heap::free(void* ptr) {
    Heap_block_bitmap* b;
    uint64_t ptroff;
    uint32_t bi, x;
    uint8_t* bm;
    uint8_t id;
    uint32_t max;

    for (b = fblock; b; b = b->next) {
        if ((uint64_t)ptr > (uint64_t)b && (uint64_t)ptr < (uint64_t)b + sizeof(Heap_block_bitmap) + b->size) {
            /* found block */
            ptroff = (uint64_t)ptr - (uint64_t)&b[1]; /* get offset to get block */
            /* block offset in BM */
            bi = ptroff / b->bsize;
            /* .. */
            bm = (uint8_t*)&b[1];
            /* clear allocation */
            id = bm[bi];
            /* oddly.. GCC did not optimize this */
            max = b->size / b->bsize;
            for (x = bi; bm[x] == id && x < max; ++x) {
                bm[x] = 0;
            }
            /* update free block count */
            b->used -= x - bi;
            return;
        }
    }

    /* this error needs to be raised or reported somehow */
    return;
}
