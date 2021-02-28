//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* these tests are used by other tests, of block devices like ata, partition, ramdisk...
*/
#ifndef __TEST_BLOCKDEVICE_H
#define __TEST_BLOCKDEVICE_H

struct object;

void test_block_device(struct object* dev);

#endif