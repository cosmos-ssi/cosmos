cmake_minimum_required(version 3.16)
project(kernel languages C ASM_NASM)

# includes
include("cmake/load_platform.cmake")
include("cmake/cflags.cmake")
include("cmake/nasmflags.cmake")

# load our platform specific sources and flags
load_platform("${TARGET_PLATFORM}")

# process generic sources
file(
  glob_recurse
  GENERIC_SRCS
  configure_depends
  "*.c"
  "*.h"
  "*.asm"
)
list(remove_item GENERIC_SRCS ${ALL_PLATFORM_SRCS}})

# build kernel target
add_library(kernel static ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories(kernel private .)
target_compile_options(kernel private $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}> $<$<COMPILE_LANGUAGE:ASM_NASM>:${NASM_FLAGS}>)
target_link_options(kernel PRIVATE "-T ${CMAKE_SOURCE_DIR}/${PLATFORM_LAYOUT}")
