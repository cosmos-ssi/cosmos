include ../../mk/mtools.mk
include ../../mk/build.mk

FAT16_ROOT=root.img
MTEMP=$(PWD)/.tmp_mtools
ROOT_FS_TEMP=./.rootfs

all: buildfs mkfat16 copyfiles dirfiles

mkmtemp:
	echo 'drive c: file="./$(FAT16_ROOT)" partition=1' > $(MTEMP)

buildfs:
	$(RM) -r $(ROOT_FS_TEMP)
	mkdir $(ROOT_FS_TEMP)
	cp -R ../fs/rootfs/* $(ROOT_FS_TEMP)
	mkdir $(ROOT_FS_TEMP)/lib
	cp ../lib/libcosmos.a $(ROOT_FS_TEMP)/lib/
	mkdir $(ROOT_FS_TEMP)/bin
	cp ../apps/init/init $(ROOT_FS_TEMP)/bin/
	cp ../apps/term/term $(ROOT_FS_TEMP)/bin/

mkfat16: mkmtemp
	$(DD) if=/dev/zero of=$(FAT16_ROOT) bs=512 count=20000 && \
	export MTOOLSRC=$(MTEMP) && \
	$(MPARTITION) -v -I -s 63 -t 20 -h 16 c: && \
	$(MPARTITION) -capv -s 63 -t 20 -h 16 c: && \
	$(MFORMAT) -v cosmos c:

copyfiles: mkmtemp
	export MTOOLSRC=$(MTEMP) && \
	$(MCOPY) -spmv $(ROOT_FS_TEMP)/* c:
	cp $(FAT16_ROOT) ../../img/

dirfiles: mkmtemp
	export MTOOLSRC=$(MTEMP) && \
	$(MDIR) -/ -a c:

clean:
	$(RM) $(MTEMP)
	$(RM) $(FAT16_ROOT)
	$(RM) -r $(ROOT_FS_TEMP)