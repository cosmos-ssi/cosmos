cmake_minimum_required(VERSION 3.16)
project(initrd)

# run the utility to create the initrd.img file, and expose the output as a file
set(INITRD_FILE initrd.img)

add_custom_command(
  OUTPUT ${INITRD_FILE}
  COMMAND $<TARGET_FILE:mkinitrd> /home/jschnurr/dev/cosmos/src/user/fs/initfs/ #FIXME
  DEPENDS mkinitrd
  COMMENT "Running mkinitrd to create the initrd.img file"
)

# create a target for the output of the custom command.
add_custom_target(_initrd ALL DEPENDS ${INITRD_FILE})

# Create an library target out of the output file
add_library(
  initrd
  STATIC
  IMPORTED
  GLOBAL
)
add_dependencies(initrd _initrd)

# Specify where the library is located
set_target_properties(initrd PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${INITRD_FILE})
