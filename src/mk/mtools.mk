#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                            *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# detect build platform
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	MCOPY=mcopy
	MMD=mmd
	MFORMAT=mformat
else
	MCOPY=mcopy
	MMD=mmd
	MFORMAT=mformat
endif

