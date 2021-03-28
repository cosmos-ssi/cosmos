include ../../mk/flags.mk
include ../../mk/build.mk

COSMOS_VIDEO_ARCHIVE=libvideo.a

LIB=../lib
EXTRA_CFLAGS= -I. -I$(LIB)/

SRC_DIR=.

all: $(COSMOS_VIDEO_ARCHIVE)

# sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h')
SRCS_CPP=$(shell find $(SRC_DIR) -type f -iname '*.cpp')
HEADERS_CPP=$(shell find $(SRC_DIR) -type f -iname '*.hpp')
SRCS_ASM=$(shell find $(SRC_DIR) -type f -iname '*.asm')

# object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_CPP=$(subst $(SRC_DIR)/, , $(SRCS_CPP:.cpp=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_CPP) $(OBJS_ASM)

$(COSMOS_VIDEO_ARCHIVE): $(OBJS)
	$(AR) crv $(COSMOS_VIDEO_ARCHIVE) $(OBJS)

# set search paths
vpath %.c .
vpath %.h .
vpath %.cpp .
vpath %.hpp .
vpath %.asm .

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# CPP files	
%.o: %.cpp $(HEADERS_CPP)
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# asm files	
%.o: %.asm
	$(NASM) $(NASMARGS) $< -o $@ 
	
clean:
	$(RM) *.o
	$(RM) $(OBJS)
	$(RM) $(COSMOS_VIDEO_ARCHIVE)
