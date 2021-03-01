#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

NASM=nasm
NASMARGS=-O0 -f elf64 -g -F dwarf
NASMARGSFLAT= -O0 -f bin

RM=rm -f
MAKE=make
DD=dd
MAPFILE=cosmos.map
LINKER_SCRIPT=cosmos.ld

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

IWYU_FIX_INCLUDES := $(shell command -v fix_include || command -v fix_includes.py || command -v fix_includes)

# detect build platform
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
include $(SELF_DIR)/gcc-i386-darwin.mk
#include $(SELF_DIR)/gcc-arm-darwin.mk

# not quite yet
#include $(SELF_DIR)/clang-i386-darwin.mk
else
include $(SELF_DIR)/gcc-i386-linux.mk
endif

include $(SELF_DIR)/qemu.mk
