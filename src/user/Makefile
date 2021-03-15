include ../mk/flags.mk
include ../mk/build.mk
include ../mk/mtools.mk

COSMOS_INITRD_IMAGE=initrd.img
COSMOS_LIB_ARCHIVE=lib/libcosmos.a
COSMOS_INIT=apps/init/init
COSMOS_TERM=apps/term/term
COSMOS_TEST_BINARY=fs/rootfs/test.bin
COSMOS_TEST_BINARY_ELF=fs/rootfs/test.elf
MKINITRD=../util/mkinitrd/mkinitrd

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_INITRD_IMAGE) rootfs

$(COSMOS_LIB_ARCHIVE):
	make -C lib

$(COSMOS_TEST_BINARY): fs/rootfs/test.asm
	$(NASM) $(NASMARGSFLAT) -o $(COSMOS_TEST_BINARY) fs/rootfs/test.asm

$(COSMOS_TEST_BINARY_ELF): fs/rootfs/test.asm
	$(NASM) $(NASMARGS) -o fs/rootfs/test.o fs/rootfs/test.asm
	$(LD) -m elf_x86_64 -o $(COSMOS_TEST_BINARY_ELF) fs/rootfs/test.o

$(COSMOS_INITRD_IMAGE): $(COSMOS_LIB_ARCHIVE) $(COSMOS_TEST_BINARY) $(COSMOS_TEST_BINARY_ELF)
	$(RM) $(COSMOS_INITRD_IMAGE)
	make -C apps
	$(MKINITRD) fs/initfs/ 
	mv $(COSMOS_INITRD_IMAGE) ../img/

rootfs:
	cd mkroot ; make clean ; make

clean:
	$(RM) -r *.o
	$(RM) -r *.a
	$(RM) -r *.bin
	$(RM) $(COSMOS_TEST_BINARY)
	$(RM) $(COSMOS_INITRD_IMAGE)
	cd mkroot; make clean
	cd apps; make clean
	cd lib; make clean

