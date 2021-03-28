include ../mk/flags.mk
include ../mk/build.mk
include ../mk/mtools.mk

COSMOS_INITRD_IMAGE=initrd.img
COSMOS_LIB_ARCHIVE=lib/libcosmos.a
COSMOS_VIDEO_ARCHIVE=video/libvideo.a
COSMOS_GUI_ARCHIVE=gui/libgui.a

MKINITRD=../util/mkinitrd/mkinitrd

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_INITRD_IMAGE) rootfs

$(COSMOS_LIB_ARCHIVE):
	make -C lib

$(COSMOS_VIDEO_ARCHIVE):
	make -C video

$(COSMOS_GUI_ARCHIVE):
	make -C gui

$(COSMOS_INITRD_IMAGE): $(COSMOS_LIB_ARCHIVE) $(COSMOS_VIDEO_ARCHIVE)
	$(RM) $(COSMOS_INITRD_IMAGE)
	make -C apps
	cp apps/test/test.bin fs/rootfs/
	cp apps/test/test.elf fs/rootfs/
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
	cd video; make clean
	cd gui; make clean