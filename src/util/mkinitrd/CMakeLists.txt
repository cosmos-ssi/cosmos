cmake_minimum_required(version 3.16)
project(initrd languages C)

file(
  glob_recurse
  SRCS
  configure_depends
  "*.c"
  "*.h"
)

# build target
add_library(initrd static ${SRCS})
