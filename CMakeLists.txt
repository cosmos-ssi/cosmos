cmake_minimum_required(version 3.16)
project(CosmOS version 1.0)

set(TARGET_PLATFORM "x86-64")

add_subdirectory(src/boot)
add_subdirectory(src/kernel)
add_subdirectory(src/util/mkinitrd)

# define a custom command to concatenate the object files in sequence into a bootimage.
set(HDA_FILE hda.img)
add_custom_command(
  output ${HDA_FILE}
  # init the file
  command test -f $(HDA_FILE) || $(DD) if=/dev/zero of=${HDA_FILE} bs=32768 count=129024
  # write boot loader
  command dd if=$<TARGET_FILE:bootimage> of=${HDA_FILE} conv=notrunc bs=512 status=none
  # # write kernel
  command dd if=$<TARGET_FILE:kernel> of=${HDA_FILE} conv=notrunc bs=512 seek=4
  # # write initrd fs at offset of 10MB
  command dd if=$<TARGET_FILE:initrd> of=${HDA_FILE} conv=notrunc bs=512 seek=20480
  # wait for dependencies
  depends bootimage kernel initrd
  comment "Building ${HDA_FILE}"
)

# create a target for the output of the custom command.
add_custom_target(hda all depends ${HDA_FILE})
