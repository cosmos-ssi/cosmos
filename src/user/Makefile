include ../mk/flags.mk
include ../mk/build.mk

COSMOS_INITRD_IMAGE=initrd.img
COSMOS_ABI_ARCHIVE=cosmos_abi.a
COSMOS_INIT=cosmos_init
COSMOS_TEST_BINARY=initfs/test.bin

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_INITRD_IMAGE)

# ABI sources
SRCS_C=$(shell find $(SRC_DIR)/ -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR)/ -type f -iname '*.h')
SRCS_ASM=$(shell find $(SRC_DIR)/ -type f -iname '*.asm' ! -name 'test.asm')

# ABI object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

init/init.o: init/init.h init/init.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o init/init.o init/init.c

initfs/test.bin: initfs/test.asm
	$(NASM) $(NASMFLATARGS) -o initfs/test.bin initfs/test.asm

$(COSMOS_INITRD_IMAGE): $(COSMOS_INIT) $(COSMOS_ABI_ARCHIVE)  $(COSMOS_TEST_BINARY)
	rm -f $(COSMOS_INITRD_IMAGE)
	../util/mkinitrd/mkinitrd cosmos_abi.a cosmos_init initfs/welcome.txt initfs/testdata.txt initfs/test.bin initfs/ocean.bmp initfs/zap-vga16.psf
	mv initrd.img ../img/

$(COSMOS_INIT): $(COSMOS_ABI_ARCHIVE) init/init.o
	$(LD) -m elf_x86_64 -o $(COSMOS_INIT) $(COSMOS_ABI_ARCHIVE)

$(COSMOS_ABI_ARCHIVE): abi/cosmos_abi.o abi/cosmos_syscall.o
	$(AR) $(ARFLAGS) $(COSMOS_ABI_ARCHIVE) abi/cosmos_abi.o abi/cosmos_syscall.o

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
	$(RM) -r $(OBJS)
	$(RM) $(COSMOS_ABI_ARCHIVE)
	$(RM) $(COSMOS_INIT)
	$(RM) $(COSMOS_INITRD_IMAGE)
	$(RM) ../img/$(COSMOS_INITRD_IMAGE)
