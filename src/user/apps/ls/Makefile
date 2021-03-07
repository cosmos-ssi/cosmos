include ../../../mk/flags.mk
include ../../../mk/build.mk
include ../user.mk

LS=ls

SRC_DIR=.

all: $(LS)

ls.o: ls.hpp ls.cpp
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o ls.o ls.cpp

$(LS): $(COSMOS_ABI_ARCHIVE) ls.o
	$(LD) -m elf_x86_64 -o $(LS) ls.o $(COSMOS_ABI_ARCHIVE)  
	
clean:
	$(RM) *.o
	$(RM) $(LS)
