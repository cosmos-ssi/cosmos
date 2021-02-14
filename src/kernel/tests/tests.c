//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/tests.h>
#include <tests/dev/test_ata.h>
#include <tests/dev/test_bda.h>
#include <tests/dev/test_null.h>
#include <tests/dev/test_ramdisk.h>
#include <tests/dev/test_rand.h>
#include <tests/dev/test_smbios.h>
#include <tests/fs/test_devfs.h>
#include <tests/fs/test_gpt.h>
#include <tests/fs/test_initrd.h>
#include <tests/fs/test_swap.h>
#include <tests/fs/test_vfs.h>
#include <tests/sys/test_array.h>
#include <tests/sys/test_arraylist.h>
#include <tests/sys/test_bitmap.h>
#include <tests/sys/test_init_loader.h>
#include <tests/sys/test_iobuffers.h>
#include <tests/sys/test_linkedlist.h>
#include <tests/sys/test_malloc.h>
#include <tests/sys/test_ntree.h>
#include <tests/sys/test_objects.h>
#include <tests/sys/test_ringbuffer.h>
#include <tests/sys/test_string.h>
#include <tests/sys/test_tree.h>

void tests_run() {
    test_malloc();
    test_array();
    test_arraylist();
    test_ringbuffer();
    test_linkedlist();
    test_tree();
    test_ntree();
    test_string();
    test_bitmap();
    test_iobuffers();
    test_vfs();
    test_devfs();
    test_gpt();
    test_bda();
    test_smbios();
    test_ramdisk();
    test_swap();
    test_rand();
    test_null();
    test_initrd();
    test_ata();
    test_init_loader();
    test_objects();
}
