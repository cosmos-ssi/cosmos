//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJ_H
#define _OBJ_H

#include <obj/logical/fs/objfs/objfs.h>
#include <obj/logical/fs/swap/swap.h>
#include <obj/logical/ramdisk/ramdisk.h>
#include <obj/logical/virtio/virtio.h>

#ifdef TARGET_PLATFORM_i386

#else

#include <obj/arm/pl101/pl101.h>

#endif

#endif