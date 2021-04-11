cmake_minimum_required(VERSION 3.16)
project(kernel LANGUAGES C ASM_NASM)

# Populate PLATFORM_SRCS and _IGNORE
include("cmake/platform_sources.cmake")
load_platform_sources(${CMAKE_SYSTEM_PROCESSOR})

# Populate GENERIC_SRCS
file(
  GLOB_RECURSE
  GENERIC_SRCS
  CONFIGURE_DEPENDS
  "*.c"
  "*.h"
  "*.asm"
)
list(REMOVE_ITEM GENERIC_SRCS ${_IGNORE}})

# Configure kernel compile options
add_compile_options(
  $<$<COMPILE_LANGUAGE:C>:-O0>
  $<$<COMPILE_LANGUAGE:C>:-gdwarf-4>
  $<$<COMPILE_LANGUAGE:C>:-fPIC>
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

# compile the kernel into a static library first. This helps the linker find symbols in the next step.
add_library(libkernel STATIC ${PLATFORM_SRCS} ${GENERIC_SRCS})
target_include_directories(libkernel PRIVATE .)
target_compile_definitions(libkernel PRIVATE COMPILE_PLATFORM_${CMAKE_HOST_SYSTEM_NAME}) # Linux or Darwin
target_compile_definitions(libkernel PRIVATE TARGET_PLATFORM_${CMAKE_SYSTEM_PROCESSOR})

# compile into an elf executable
add_executable(kernel cosmos.c)
target_include_directories(kernel PRIVATE .)
target_link_libraries(kernel libkernel)
set_target_properties(kernel PROPERTIES SUFFIX .bin)
target_link_options(
  kernel
  PRIVATE
  -nostdlib
  -nolibc
  -Wl,-T,${PROJECT_SOURCE_DIR}/cosmos.ld
  -Wl,-Map,${CMAKE_CURRENT_BINARY_DIR}/cosmos.map
  -Wl,--no-relax
)

# Extract the binary image from the ELF file to a kernel image.
add_custom_command(
  TARGET kernel
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kernel> kernel.elf
  COMMAND ${CMAKE_OBJCOPY} kernel.elf -O binary $<TARGET_FILE:kernel>
  BYPRODUCTS kernel.elf
  COMMENT "Convert the ELF output file to a binary image"
)
