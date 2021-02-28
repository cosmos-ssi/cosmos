//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/tfs/tfs_block.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/string/mem.h>

/*
 * read the superblock at lba 0
 */
void tfs_read_superblock(struct object* dev, struct tfs_superblock_block* superblock) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(superblock);
    blockutil_read(dev, (uint8_t*)superblock, sizeof(struct tfs_superblock_block), 0, 0);
}

/*
 * write the superblock at lba 0
 */
void tfs_write_superblock(struct object* dev, struct tfs_superblock_block* superblock) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(superblock);
    blockutil_write(dev, (uint8_t*)superblock, sizeof(struct tfs_superblock_block), 0, 0);
}

/*
 * write a dir block
 */
void tfs_write_dir_block(struct object* dev, struct tfs_dir_block* dir_block, uint64_t lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dir_block);
    blockutil_write(dev, (uint8_t*)dir_block, sizeof(struct tfs_dir_block), lba, 0);
}

/*
 * read a dir block
 */
void tfs_read_dir_block(struct object* dev, struct tfs_dir_block* dir_block, uint64_t lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dir_block);
    blockutil_read(dev, (uint8_t*)dir_block, sizeof(struct tfs_dir_block), lba, 0);
}

void tfs_write_map_block(struct object* dev, struct tfs_map_block* map_block, uint64_t lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(map_block);
    blockutil_write(dev, (uint8_t*)map_block, sizeof(struct tfs_map_block), lba, 0);
}

void tfs_read_map_block(struct object* dev, struct tfs_map_block* map_block, uint64_t lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(map_block);
    blockutil_read(dev, (uint8_t*)map_block, sizeof(struct tfs_map_block), lba, 0);
}

void tfs_write_file_block(struct object* dev, struct tfs_file_block* file_block, uint64_t lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(file_block);
    blockutil_write(dev, (uint8_t*)file_block, sizeof(struct tfs_file_block), lba, 0);
}

void tfs_read_file_block(struct object* dev, struct tfs_file_block* file_block, uint64_t lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(file_block);
    blockutil_read(dev, (uint8_t*)file_block, sizeof(struct tfs_file_block), lba, 0);
}
