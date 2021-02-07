#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                            *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

COMMON_CFLAGS=-c -m64 -mno-red-zone -ffreestanding -Werror -Wuninitialized -Wunused-variable -Wreturn-type -Wparentheses -fPIC -O0 -g3