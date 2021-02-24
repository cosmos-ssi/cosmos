include ../../mk/flags.mk
include ../../mk/build.mk

COSMOS_LIB_ARCHIVE=cosmos_lib.a

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_LIB_ARCHIVE)

# ABI sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h')
SRCS_CPP=$(shell find $(SRC_DIR) -type f -iname '*.cpp')
HEADERS_CPP=$(shell find $(SRC_DIR) -type f -iname '*.hpp')
SRCS_ASM=$(shell find $(SRC_DIR) -type f -iname '*.asm')

# ABI object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_CPP=$(subst $(SRC_DIR)/, , $(SRCS_CPP:.cpp=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_CPP) $(OBJS_ASM)

$(COSMOS_LIB_ARCHIVE): $(OBJS)
	$(AR) $(ARFLAGS) $(COSMOS_LIB_ARCHIVE) $(OBJS)

# set search paths
vpath %.c .
vpath %.h .
vpath %.cpp .
vpath %.hpp .
vpath %.asm .

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# CPP files	
%.o: %.cpp $(HEADERS_CPP)
	$(CC) $(CPPFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# asm files	
%.o: %.asm
	$(NASM) $(NASMARGS) $< -o $@ 
	
clean:
	$(RM) -r *.o
	$(RM) $(COSMOS_LIB_ARCHIVE)
