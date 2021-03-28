//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <object/filesystem/filesystem_object.hpp>

struct fs_node* FilesystemObject::getRootNode() {
    return 0;
}

uint32_t FilesystemObject::read(struct fs_node* node, uint8_t* data, uint32_t data_size) {
    return 0;
}

uint32_t FilesystemObject::write(struct fs_node* node, const uint8_t* data, uint32_t data_size) {
    return 0;
}

void FilesystemObject::open(struct fs_node* node) {}
void FilesystemObject::close(struct fs_node* node) {}

struct fs_node* FilesystemObject::findNodeById(struct fs_node* node, uint64_t id) {
    return 0;
}

void FilesystemObject::listDirectory(struct fs_node* node, struct filesystem_directory* dir) {}

uint64_t FilesystemObject::size(struct fs_node* node) {
    return 0;
}

uint8_t* FilesystemObject::read_file(uint8_t* device_name, uint8_t* file_name, uint32_t* len) {
    return 0;
}
