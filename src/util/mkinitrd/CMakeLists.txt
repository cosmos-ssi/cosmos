cmake_minimum_required(VERSION 3.16)
project(initrd LANGUAGES C)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
    "*.c"
    "*.h"
)

# build target
add_library(initrd STATIC ${SRCS})
