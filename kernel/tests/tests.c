//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/tests.h>

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
    test_gpt();
    test_bda();
    test_smbios();
    test_ramdisk();
    test_swap();
    test_rand();
    test_null();
    test_initrd();
    test_ata();
}
