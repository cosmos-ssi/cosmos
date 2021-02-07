include ../scripts/mk/flags.mk
include ../scripts/mk/build.mk

COSMOS_ABI_ARCHIVE=cosmos_abi.a
COSMOS_INIT=cosmos_init

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_ABI_ARCHIVE) $(COSMOS_INIT)

# ABI sources
SRCS_C=$(shell find $(SRC_DIR)/abi -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR)/abi -type f -iname '*.h')
SRCS_ASM=$(shell find $(SRC_DIR)/abi -type f -iname '*.asm')

# ABI object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

init/init.o: init/init.h init/init.C 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(COSMOS_INIT): $(COSMOS_ABI_ARCHIVE) init/init.o
	$(LD) -m elf_x86_64 -o $(COSMOS_INIT) $(COSMOS_ABI_ARCHIVE)

$(COSMOS_ABI_ARCHIVE): $(OBJS)
	$(AR) $(ARFLAGS) $(COSMOS_ABI_ARCHIVE) $(OBJS)

# set search paths
vpath %.c .
vpath %.h .
vpath %.asm .

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# asm files	
%.o: %.asm
	$(NASM) $(NASMARGS) $< -o $@ 
	
clean:
	$(RM) init/init.o
	$(RM) $(OBJS)
	$(RM) $(COSMOS_ABI_ARCHIVE)
	$(RM) $(COSMOS_INIT)
