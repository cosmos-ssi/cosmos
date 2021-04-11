# A CMake toolchain file so we can cross-compile for Generic x86_64
# inspired by https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-3/

# usage
# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-gcc-x86-linux.cmake ../

# The Generic system name is used for embedded targets (targets without OS) in CMake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set a toolchain path. You only need to set this if the toolchain isn't in your system path.
if($TC_PATH)
  string(APPEND TC_PATH "/")
endif()

# The toolchain prefix for all toolchain executables
set(CROSS_COMPILE)

# specify the cross compiler
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_ASM_NASM_COMPILER nasm)

# Because the cross-compiler cannot directly generate a binary without complaining, just test
# compiling a static library instead of an executable program
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# We must set the OBJCOPY setting into cache so that it's available to the
# whole project. Otherwise, this does not get set into the CACHE and therefore
# the build doesn't know what the OBJCOPY filepath is

set(CMAKE_OBJCOPY
    ${TC_PATH}${CROSS_COMPILE}objcopy
    CACHE FILEPATH "The toolchain objcopy command " FORCE
)

set(CMAKE_OBJDUMP
    ${TC_PATH}${CROSS_COMPILE}objdump
    CACHE FILEPATH "The toolchain objdump command " FORCE
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
