#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                            *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# detect build platform
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	MTOOLS=/usr/local/bin/mtools
	DOSFSTOOLS_MKFS_MSDOS=/usr/local/Cellar/dosfstools/4.2/sbin/mkfs.msdos
else
	MTOOLS=/usr/local/bin/mtools
	DOSFSTOOLS_MKFS_MSDOS=/usr/local/Cellar/dosfstools/4.2/sbin/mkfs.msdos
endif

