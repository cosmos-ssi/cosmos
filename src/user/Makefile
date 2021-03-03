include ../mk/flags.mk
include ../mk/build.mk
include ../mk/mtools.mk

COSMOS_ROOT_IMAGE=root.img
COSMOS_INITRD_IMAGE=initrd.img
COSMOS_LIB_ARCHIVE=lib/libcosmos.a
COSMOS_INIT=apps/init/init
COSMOS_TERM=apps/term/term
COSMOS_TEST_BINARY=fs/initfs/test.bin
MKINITRD=../util/mkinitrd/mkinitrd

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_INITRD_IMAGE) $(COSMOS_ROOT_IMAGE)

$(COSMOS_LIB_ARCHIVE):
	make -C lib

$(COSMOS_TEST_BINARY): fs/initfs/test.asm
	$(NASM) $(NASMARGSFLAT) -o $(COSMOS_TEST_BINARY) fs/initfs/test.asm

$(COSMOS_INITRD_IMAGE): $(COSMOS_LIB_ARCHIVE)  $(COSMOS_TEST_BINARY)
	$(RM) $(COSMOS_INITRD_IMAGE)
	make -C apps
	$(MKINITRD) fs/initfs/ $(COSMOS_INIT) $(COSMOS_LIB_ARCHIVE) $(COSMOS_TEST_BINARY) 
	mv $(COSMOS_INITRD_IMAGE) ../img/

$(COSMOS_ROOT_IMAGE):
	$(RM) $(COSMOS_ROOT_IMAGE)
	$(DD) if=/dev/zero of=$(COSMOS_ROOT_IMAGE) bs=32768 count=32768
	$(DOSFSTOOLS_MKFS_MSDOS) -F 16 $(COSMOS_ROOT_IMAGE)
	$(MCOPY) -s -i $(COSMOS_ROOT_IMAGE) fs/rootfs/* ::/
	mv $(COSMOS_ROOT_IMAGE) ../img/

clean:
	$(RM) -r *.o
	$(RM) -r *.a
	$(RM) -r *.bin
	$(RM) $(COSMOS_TEST_BINARY)
	$(RM) $(COSMOS_INITRD_IMAGE)
	$(RM) $(COSMOS_ROOT_IMAGE)
	cd apps; make clean
	cd lib; make clean

