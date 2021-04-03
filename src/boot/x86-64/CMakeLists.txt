cmake_minimum_required(version 3.16)
project("bootloader-x86-64")

# set flags
set(CMAKE_ASM_NASM_OBJECT_FORMAT bin)
add_compile_options(-O0)

# let cmake know to expect NASM assembler files
enable_language(ASM_NASM)

# build object files individually
add_library(boot object boot.asm)
add_library(boot2 object boot2.asm)
add_library(boot3 object boot3.asm)

# define a custom command to concatenate the object files in sequence into a bootimage.
set(BOOTIMAGE_FILE bootimage.bin)
add_custom_command(
  output ${BOOTIMAGE_FILE}
  # write boot loader
  command dd if=$<TARGET_OBJECTS:boot> of=${BOOTIMAGE_FILE} conv=notrunc bs=512 status=none
  command dd if=$<TARGET_OBJECTS:boot2> of=${BOOTIMAGE_FILE} conv=notrunc bs=512 seek=1 status=none
  command dd if=$<TARGET_OBJECTS:boot3> of=${BOOTIMAGE_FILE} conv=notrunc bs=512 seek=3 status=none
  # wait for dependencies
  depends boot boot2 boot3
  comment "Building ${BOOTIMAGE_FILE}"
)

# create a target for the output of the custom command.
add_custom_target(bootimage_target all depends ${BOOTIMAGE_FILE})

# Create an library target out of the library compilation result
add_library(
  bootimage
  static
  imported
  global
)
add_dependencies(bootimage bootimage_target)

# Specify where the library is located
set_target_properties(bootimage properties IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${BOOTIMAGE_FILE})
