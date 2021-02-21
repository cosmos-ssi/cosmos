//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs_facade.h>
#include <sys/gui/bmp.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct bmp* bmp_new() {
    struct bmp* ret = kmalloc(sizeof(struct bmp));
    memzero((uint8_t*)ret, sizeof(struct bmp));
    return ret;
}

void bmp_load(struct bmp* bitmap, uint8_t* devname, uint8_t* filename) {
    ASSERT_NOT_NULL(devname);
    ASSERT_NOT_NULL(filename);
    ASSERT_NOT_NULL(bitmap);
    /*
    * delete previous bmp if there is one
    */
    if (0 != bitmap->buffer) {
        kfree(bitmap->buffer);
        bitmap->buffer = 0;
        bitmap->buffer_size = 0;
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
            bitmap->buffer_size = fsfacade_size(fs_node);
            uint8_t* buffer = kmalloc(bitmap->buffer_size);
            fsfacade_read(fs_node, buffer, bitmap->buffer_size);
            bitmap->buffer = buffer;
            kprintf("read done\n");
        } else {
            kprintf("Unable to find file %s for GUI\n", filename);
        }
    } else {
        kprintf("Unable to find fs device %s for GUI\n", devname);
    }
}
void bmp_delete(struct bmp* bitmap) {
    ASSERT_NOT_NULL(bitmap);
    if (0 != bitmap->buffer) {
        kfree(bitmap->buffer);
    }
    kfree(bitmap);
}
