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

# compile the kernel into an archive
add_library(kernel_archive STATIC ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories(kernel_archive PRIVATE .)
target_compile_options(
  kernel_archive
  PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC>
          $<$<COMPILE_LANGUAGE:C>:-O0>
          $<$<COMPILE_LANGUAGE:C>:-gdwarf-4>
          $<$<COMPILE_LANGUAGE:C>:-c>
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
target_compile_definitions(kernel_archive PRIVATE COMPILE_PLATFORM_${CMAKE_HOST_SYSTEM_NAME})
target_compile_definitions(kernel_archive PRIVATE TARGET_PLATFORM_${CMAKE_SYSTEM_PROCESSOR})

# link the kernel archive into an elf file
set(LD_FLAGS
    -m
    elf_x86_64
    -nostdlib
    --no-relax
)
set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/cosmos.ld)
set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/cosmos.map)
set(ELF_FILE ${CMAKE_CURRENT_BINARY_DIR}/cosmos.elf)

add_custom_target(
  kernel_elf ALL
  BYPRODUCTS ${ELF_FILE} ${MAPFILE}
  COMMAND ${CMAKE_LD} ${LD_FLAGS} -T ${LINKER_SCRIPT} -Map ${MAP_FILE} -o ${ELF_FILE} $<TARGET_FILE:kernel_archive>
  DEPENDS kernel_archive
  COMMENT "Building ${ELF_FILE}"
  VERBATIM
)

# objcopy the elf into a bin file
set(OBJCOPY_FLAGS -O binary)
set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/cosmos.bin)

add_custom_target(
  kernel_bin ALL
  BYPRODUCTS ${BIN_FILE}
  COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_FLAGS} ${ELF_FILE} ${BIN_FILE}
  DEPENDS kernel_elf
  COMMENT "Building ${BIN_FILE}"
  VERBATIM
)

# Create a library target for the kernel bin
add_library(
  kernel
  STATIC
  IMPORTED
  GLOBAL
)
add_dependencies(kernel kernel_bin)

# Specify where the library is located
set_target_properties(kernel PROPERTIES IMPORTED_LOCATION ${BIN_FILE})
