cmake_minimum_required(VERSION 3.16)
project(kernel LANGUAGES C ASM_NASM)

# includes
include("cmake/load_platform.cmake")
include("cmake/cflags.cmake")
include("cmake/nasmflags.cmake")

add_compile_options(-Werror)

# set(CMAKE_C_FLAGS -c -m64 -mno-red-zone -ffreestanding ${warnings} ) set(CMAKE_CXX_FLAGS -c -m64 -mno-red-zone
# -ffreestanding ${warnings} ) NASMARGS=-O0 -f elf64 -g -F dwarf
#

# load our platform specific sources and flags
load_platform(${CMAKE_SYSTEM_PROCESSOR})

# process generic sources
file(
  GLOB_RECURSE
  GENERIC_SRCS
  CONFIGURE_DEPENDS
  "*.c"
  "*.h"
  "*.asm"
)
list(REMOVE_ITEM GENERIC_SRCS ${ALL_PLATFORM_SRCS}})

# build kernel target
add_library(kernel STATIC ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories(kernel PRIVATE .)
target_compile_options(kernel PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}> $<$<COMPILE_LANGUAGE:ASM_NASM>:${NASM_FLAGS}>)
target_link_options(kernel PRIVATE "-T ${CMAKE_SOURCE_DIR}/${PLATFORM_LAYOUT}")
