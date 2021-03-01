include ../mk/flags.mk
include ../mk/build.mk

COSMOS_INITRD_IMAGE=initrd.img
COSMOS_LIB_ARCHIVE=lib/libcosmos.a
COSMOS_INIT=apps/init/init
COSMOS_TERM=apps/term/term
COSMOS_TEST_BINARY=fs/initfs/test.bin
MKINITRD=../util/mkinitrd/mkinitrd

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_INITRD_IMAGE)

$(COSMOS_LIB_ARCHIVE):
	make -C lib

$(COSMOS_TEST_BINARY): fs/initfs/test.asm
	$(NASM) $(NASMARGSFLAT) -o $(COSMOS_TEST_BINARY) fs/initfs/test.asm

$(COSMOS_INITRD_IMAGE): $(COSMOS_LIB_ARCHIVE)  $(COSMOS_TEST_BINARY)
	make -C apps
	$(MKINITRD) fs/initfs/ $(COSMOS_INIT) $(COSMOS_LIB_ARCHIVE) $(COSMOS_TEST_BINARY) 
	mv initrd.img ../img/
	
clean:
	$(RM) -r *.o
	$(RM) -r *.a
	$(RM) -r *.bin
	$(RM) $(COSMOS_TEST_BINARY)
	$(RM) $(COSMOS_INITRD_IMAGE)
	cd apps; make clean
	cd lib; make clean

