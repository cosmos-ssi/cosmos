cmake_minimum_required(VERSION 3.16)
project(kernel LANGUAGES C ASM_NASM)

# Sources
## populate PLATFORM_SRCS and _IGNORE
include("cmake/platform_sources.cmake")
load_platform_sources(${CMAKE_SYSTEM_PROCESSOR})

## populate GENERIC_SRCS
file(
  GLOB_RECURSE
  GENERIC_SRCS
  CONFIGURE_DEPENDS
  "*.c"
  "*.h"
  "*.asm"
)
list(REMOVE_ITEM GENERIC_SRCS ${_IGNORE}})

# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(CMAKE_C_LINK_EXECUTABLE
#     "<${CMAKE_LD}> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>"
# )

# compile the kernel into an archive
add_executable(kernel ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories(kernel PRIVATE .)
target_compile_options(
  kernel
  PRIVATE $<$<COMPILE_LANGUAGE:C>:-O0>
          $<$<COMPILE_LANGUAGE:C>:-gdwarf-4>
          $<$<COMPILE_LANGUAGE:C>:-fPIC>
          # $<$<COMPILE_LANGUAGE:C>:-c>
          # $<$<COMPILE_LANGUAGE:C>:-shared>
          $<$<COMPILE_LANGUAGE:C>:-m64>
          $<$<COMPILE_LANGUAGE:C>:-mno-red-zone>
          $<$<COMPILE_LANGUAGE:C>:-ffreestanding>
          $<$<COMPILE_LANGUAGE:C>:-Werror>
          $<$<COMPILE_LANGUAGE:C>:-Wuninitialized>
          $<$<COMPILE_LANGUAGE:C>:-Wunused-variable>
          $<$<COMPILE_LANGUAGE:C>:-Wreturn-type>
          $<$<COMPILE_LANGUAGE:C>:-Wparentheses>
          $<$<COMPILE_LANGUAGE:ASM_NASM>:-O0>
          $<$<COMPILE_LANGUAGE:ASM_NASM>:-gdwarf>
)

## set compile definitions
## CMAKE_HOST_SYSTEM_NAME resolves to Linux or Darwin depending on build system
target_compile_definitions(kernel PRIVATE COMPILE_PLATFORM_${CMAKE_HOST_SYSTEM_NAME})
target_compile_definitions(kernel PRIVATE TARGET_PLATFORM_${CMAKE_SYSTEM_PROCESSOR})

# configure the linker.
# the -Wl syntax is used to pass flags directly to the linker, even if gcc doesn't understand them
target_link_options(
  kernel
  PRIVATE
  -v
  -Wl,-nostdlib
  -Wl,-T,${PROJECT_SOURCE_DIR}/cosmos.ld
  -Wl,-Map,${CMAKE_CURRENT_BINARY_DIR}/cosmos.map
  -Wl,--no-relax
  -Wl,--start-group
)

# add_custom_command(
#     TARGET kernel.${TUTORIAL}.${BOARD} POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} ./kernel.${TUTORIAL}.${BOARD}${CMAKE_EXECUTABLE_SUFFIX} -O binary ./kernel.${TUTORIAL}.${BOARD}.img
#     COMMAND ${CMAKE_OBJDUMP} -l -S -D ./kernel.${TUTORIAL}.${BOARD}${CMAKE_EXECUTABLE_SUFFIX} > ./kernel.${TUTORIAL}.${BOARD}.asm
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Convert the ELF output file to a binary image" )
