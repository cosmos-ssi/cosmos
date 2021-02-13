#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                            *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# platform flags
COMPILE_PLATFORM=COMPILE_PLATFORM_DARWIN
TARGET_PLATFORM=TARGET_PLATFORM_i386

# cc (GCC tool chain)
CC=x86_64-elf-gcc
CFLAGS=$(COMMON_CFLAGS) -D$(COMPILE_PLATFORM) -D$(TARGET_PLATFORM)

# ld
LD=x86_64-elf-ld
LDFLAGS=-m elf_x86_64 -T $(LINKER_SCRIPT) -Map $(MAPFILE) -nostdlib --no-relax

# objcopy
OBJCOPY=x86_64-elf-objcopy
OBJCOPYFLAGS=-O binary

# strip
STRIP=x86_64-elf-strip
STRIPFLAGS=

# ar
AR=x86_64-elf-ar
ARFLAGS=-crs
