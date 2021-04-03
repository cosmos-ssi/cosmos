cmake_minimum_required(VERSION 3.16)
project("bootloader-x86-64")

# set flags
set(CMAKE_ASM_NASM_OBJECT_FORMAT bin)
add_compile_options(-O0)

# let cmake know to expect NASM assembler files
enable_language(ASM_NASM)

# build object files individually
add_library(boot OBJECT boot.asm)
add_library(boot2 OBJECT boot2.asm)
add_library(boot3 OBJECT boot3.asm)

# define a custom command to concatenate the object files in sequence into a bootimage.
set(BOOTIMAGE_FILE bootimage.bin)
add_custom_command(
  OUTPUT ${BOOTIMAGE_FILE}
  # write boot loader
  COMMAND dd if=$<TARGET_OBJECTS:boot> of=${BOOTIMAGE_FILE} conv=notrunc bs=512 status=none
  COMMAND dd if=$<TARGET_OBJECTS:boot2> of=${BOOTIMAGE_FILE} conv=notrunc bs=512 seek=1 status=none
  COMMAND dd if=$<TARGET_OBJECTS:boot3> of=${BOOTIMAGE_FILE} conv=notrunc bs=512 seek=3 status=none
  # wait for dependencies
  DEPENDS boot boot2 boot3
  COMMENT "Building ${BOOTIMAGE_FILE}"
)

# create a target for the output of the custom command.
add_custom_target(bootimage_target ALL DEPENDS ${BOOTIMAGE_FILE})

# Create an library target out of the library compilation result
add_library(
  bootimage
  STATIC
  IMPORTED
  GLOBAL
)
add_dependencies(bootimage bootimage_target)

# Specify where the library is located
set_target_properties(bootimage PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${BOOTIMAGE_FILE})
