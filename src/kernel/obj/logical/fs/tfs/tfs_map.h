//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TFS_MAP_H
#define _TFS_MAP_H

#include <obj/logical/fs/tfs/tfs_block.h>
#include <sys/objectmgr/objectmgr.h>

#include <types.h>

#define TFS_MAP_BLOCK_FREE 0
#define TFS_MAP_BLOCK_USED 1

uint32_t tfs_map_find_free_block(struct object* dev, struct tfs_superblock_block* superblock);
void tfs_map_release_block(struct object* dev, uint64_t block, struct tfs_superblock_block* superblock);
uint32_t tfs_map_block_count(struct object* dev);
#endif