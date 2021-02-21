//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs_facade.h>
#include <sys/gui/bdf.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct bdf* bdf_new() {
    struct bdf* ret = kmalloc(sizeof(struct bdf));
    memzero((uint8_t*)ret, sizeof(struct bdf));
    return ret;
}

void bdf_load(struct bdf* font, uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);
    ASSERT_NOT_NULL(font);
    /*
    * delete previous bmp if there is one
    */
    if (0 != font->buffer) {
        kfree(font->buffer);
        font->buffer = 0;
        font->buffer_size = 0;
    }

    /*
    * get device
    */
    struct device* fs_dev = devicemgr_find_device(devname);
    if (0 != fs_dev) {
        /*
        * root node
        */
        struct filesystem_node* fs_root_node = fsfacade_get_fs_rootnode(fs_dev);
        /*
        * file node
        */
        struct filesystem_node* fs_node = fsfacade_find_node_by_name(fs_root_node, filename);
        if (0 != fs_node) {
            font->buffer_size = fsfacade_size(fs_node);
            uint8_t* buffer = kmalloc(font->buffer_size);
            fsfacade_read(fs_node, buffer, font->buffer_size);
            font->buffer = buffer;
            // debug_show_memblock(font->buffer, font->buffer_size);
        } else {
            kprintf("Unable to find file %s for Font\n", filename);
        }
    } else {
        kprintf("Unable to find fs device %s for Font\n", devname);
    }
}
void bdf_delete(struct bdf* font) {
    ASSERT_NOT_NULL(font);
    if (0 != font->buffer) {
        kfree(font->buffer);
    }
    kfree(font);
}
