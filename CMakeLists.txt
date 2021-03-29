cmake_minimum_required(VERSION 3.16)
project(CosmOS VERSION 1.0)

set(TARGET_PLATFORM "x86-64")

add_subdirectory(src/boot)
add_subdirectory(src/kernel)
add_subdirectory(src/util/mkinitrd)


# define a custom command to concatenate the object files in sequence into a bootimage.
set(HDA_FILE hda.img)
add_custom_command(
    OUTPUT ${HDA_FILE}

    # init the file
	COMMAND test -f $(HDA_FILE) || $(DD) if=/dev/zero of=${HDA_FILE} bs=32768 count=129024 
    
    # write boot loader
    COMMAND dd if=$<TARGET_FILE:bootimage> of=${HDA_FILE} conv=notrunc bs=512 status=none

	# # write kernel
	COMMAND dd if=$<TARGET_FILE:kernel> of=${HDA_FILE} conv=notrunc bs=512 seek=4

	# # write initrd fs at offset of 10MB
	COMMAND dd if=$<TARGET_FILE:initrd> of=${HDA_FILE} conv=notrunc bs=512 seek=20480

    # wait for dependencies
    DEPENDS bootimage
            kernel
            initrd

    COMMENT "Building ${HDA_FILE}"
)

# create a target for the output of the custom command.
add_custom_target(hda
    ALL     
    DEPENDS ${HDA_FILE}
)