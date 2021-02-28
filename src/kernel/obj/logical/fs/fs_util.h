//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FS_UTIL_H
#define _FS_UTIL_H

struct object;

void fsutil_attach_partition_tables(struct object* block_obj);
void fsutil_detach_partition_tables(struct object* block_obj);

void fsutil_attach_partitions(struct object* partition_table_obj);
void fsutil_detach_partitions(struct object* partition_table_obj);

void fsutil_attach_fs(struct object* partition_obj);
void fsutil_detach_fs(struct object* partition_obj);

#endif