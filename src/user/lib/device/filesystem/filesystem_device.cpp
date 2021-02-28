//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/filesystem/filesystem_device.hpp>

struct fs_node* FilesystemDevice::getRootNode() {
    return 0;
}

uint32_t FilesystemDevice::read(struct fs_node* node, uint8_t* data, uint32_t data_size) {
    return 0;
}

uint32_t FilesystemDevice::write(struct fs_node* node, const uint8_t* data, uint32_t data_size) {
    return 0;
}

void FilesystemDevice::open(struct fs_node* node) {}
void FilesystemDevice::close(struct fs_node* node) {}

struct fs_node* FilesystemDevice::findNodeById(struct fs_node* node, uint64_t id) {
    return 0;
}

void FilesystemDevice::listDirectory(struct fs_node* node, struct filesystem_directory* dir) {}

uint64_t FilesystemDevice::size(struct fs_node* node) {
    return 0;
}
