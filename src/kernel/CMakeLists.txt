cmake_minimum_required(VERSION 3.16)
project(kernel LANGUAGES C ASM_NASM)

set(TARGET_PLATFORM "x86-64")

# includes
include("cmake/load_platform.cmake")

# load our platform specific sources and flags
LOAD_PLATFORM("${TARGET_PLATFORM}")

# load all generic sources
file(GLOB_RECURSE GENERIC_SRCS CONFIGURE_DEPENDS
    "*.c"
    "*.h"
    "*.asm"
)
list (REMOVE_ITEM GENERIC_SRCS ${ALL_PLATFORM_SRCS}})

# Now set up our environment
add_library(kernel STATIC ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories (kernel PRIVATE .)

# ADD_EXECUTABLE(kernel ${PLATFORM_SRCS} ${ISA_SRCS} ${GENERIC_SRCS})
 
# SET(CMAKE_ASM-ATT_COMPILE_OBJECT 
#   "<CMAKE_ASM-ATT_COMPILER> ${ISA_ASM_FLAGS} ${PLATFORM_ASM_FLAGS} -o <OBJECT> <SOURCE>")
# SET(CMAKE_C_FLAGS "${ISA_C_FLAGS} ${PLATFORM_C_FLAGS}")
# SET_TARGET_PROPERTIES(kernel PROPERTIES LINK_FLAGS 
#   "-T ${PLATFORM_LAYOUT} -N ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")