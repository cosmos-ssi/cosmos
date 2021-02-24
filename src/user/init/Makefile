include ../../mk/flags.mk
include ../../mk/build.mk

COSMOS_INIT=cosmos_init
COSMOS_ABI_ARCHIVE=../lib/cosmos_lib.a

EXTRA_CFLAGS= -I. -I../lib/

SRC_DIR=.

all: $(COSMOS_INIT)

init.o: init.hpp init.cpp
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o init.o init.cpp

$(COSMOS_INIT): $(COSMOS_ABI_ARCHIVE) init.o
	$(LD) -m elf_x86_64 -o $(COSMOS_INIT) -L $(COSMOS_ABI_ARCHIVE) init.o   
	
clean:
	$(RM) *.o
	$(RM) $(COSMOS_INIT)
