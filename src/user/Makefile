include ../mk/flags.mk
include ../mk/build.mk

COSMOS_INITRD_IMAGE=initrd.img
COSMOS_LIB_ARCHIVE=lib/cosmos_lib.a
COSMOS_INIT=init/cosmos_init
COSMOS_TEST_BINARY=fs/initfs/test.bin
MKINITRD=../util/mkinitrd/mkinitrd

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_INITRD_IMAGE)

$(COSMOS_LIB_ARCHIVE):
	make -C lib

$(COSMOS_INIT): $(COSMOS_LIB_ARCHIVE)
	make -C init

$(COSMOS_TEST_BINARY): fs/initfs/test.asm
	$(NASM) $(NASMFLATARGS) -o $(COSMOS_TEST_BINARY) fs/initfs/test.asm

$(COSMOS_INITRD_IMAGE): $(COSMOS_INIT) $(COSMOS_LIB_ARCHIVE)  $(COSMOS_TEST_BINARY)
	$(MKINITRD) $(COSMOS_INIT) $(COSMOS_LIB_ARCHIVE)  $(COSMOS_TEST_BINARY) fs/initfs/welcome.txt fs/initfs/testdata.txt fs/initfs/cosmos.bmp fs/initfs/zap-vga16.psf
	mv initrd.img ../img/
	
clean:
	$(RM) -r *.o
	$(RM) -r *.a
	$(RM) -r *.bin
	$(RM) $(COSMOS_TEST_BINARY)
	$(RM) $(COSMOS_INITRD_IMAGE)
	cd init; make clean
	cd lib; make clean

