cmake_minimum_required(VERSION 3.16)
project(mkinitrd LANGUAGES C)

file(
  GLOB_RECURSE
  SRCS
  CONFIGURE_DEPENDS
  "*.c"
  "*.h"
)

# build the initrd utility
add_executable(mkinitrd ${SRCS})
