cmake_minimum_required(VERSION 3.16)
project(kernel LANGUAGES C ASM_NASM)

# includes
include("cmake/load_platform.cmake")
include("cmake/cflags.cmake")
include("cmake/nasmflags.cmake")

# load our platform specific sources and flags
LOAD_PLATFORM("${TARGET_PLATFORM}")

# process generic sources
file(GLOB_RECURSE GENERIC_SRCS CONFIGURE_DEPENDS
    "*.c"
    "*.h"
    "*.asm"
)
list (REMOVE_ITEM GENERIC_SRCS ${ALL_PLATFORM_SRCS}})

# build kernel target
add_library(kernel STATIC ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories (kernel PRIVATE .)
target_compile_options(kernel
  PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
      $<$<COMPILE_LANGUAGE:ASM_NASM>:${NASM_FLAGS}>
  )
target_link_options(kernel PRIVATE 
  "-T ${CMAKE_SOURCE_DIR}/${PLATFORM_LAYOUT}")
