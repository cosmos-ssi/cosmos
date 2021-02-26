include ../../../mk/flags.mk
include ../../../mk/build.mk
include ../user.mk

COSMOS_INIT=init

SRC_DIR=.

all: $(COSMOS_INIT)

init.o: init.hpp init.cpp
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o init.o init.cpp

$(COSMOS_INIT): $(COSMOS_ABI_ARCHIVE) init.o
	$(LD) -m elf_x86_64 -o $(COSMOS_INIT) init.o $(COSMOS_ABI_ARCHIVE)  
	
clean:
	$(RM) *.o
	$(RM) $(COSMOS_INIT)