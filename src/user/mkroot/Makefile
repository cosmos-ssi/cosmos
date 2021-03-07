include ../../mk/mtools.mk
include ../../mk/build.mk

FAT16_ROOT=root.img
MTEMP=$(PWD)/.tmp_mtools

all: mkfat16 copyfiles dirfiles

mkmtemp:
	echo 'drive c: file="./$(FAT16_ROOT)" partition=1' > $(MTEMP)

mkfat16: mkmtemp
	$(DD) if=/dev/zero of=$(FAT16_ROOT) bs=512 count=20000 && \
	export MTOOLSRC=$(MTEMP) && \
	$(MPARTITION) -v -I -s 63 -t 20 -h 16 c: && \
	$(MPARTITION) -capv -s 63 -t 20 -h 16 c: && \
	$(MFORMAT) c:

copyfiles: mkmtemp
	export MTOOLSRC=$(MTEMP) && \
	$(MCOPY) -spmv ../fs/rootfs/* c:
	cp $(FAT16_ROOT) ../../img/

dirfiles: mkmtemp
	export MTOOLSRC=$(MTEMP) && \
	$(MDIR) -/ -a c:

clean:
	rm $(MTEMP)
	rm $(FAT16_ROOT)