function(LOAD_PLATFORM PLATFORM)

  # enum of available target platforms
  list(
    APPEND
    PLATFORMS
    "x86_64"
    "arm"
  )

  # target platform sources
  file(
    GLOB_RECURSE
    PLATFORM_SRCS
    CONFIGURE_DEPENDS
    "sys/${PLATFORM}/*.c"
    "sys/${PLATFORM}/*.h"
    "sys/${PLATFORM}/*.asm"
    "obj/${PLATFORM}/*.c"
    "obj/${PLATFORM}/*.h"
    "obj/${PLATFORM}/*.asm"
  )

  # all platform sources (used for excludes)
  foreach(ITR ${PLATFORMS})
    file(
      GLOB_RECURSE
      _IGNORE
      CONFIGURE_DEPENDS
      "sys/${ITR}/*.c"
      "sys/${ITR}/*.h"
      "sys/${ITR}/*.asm"
      "obj/${ITR}/*.c"
      "obj/${ITR}/*.h"
      "obj/${ITR}/*.asm"
    )
    list(APPEND ALL_PLATFORM_SRCS ${_IGNORE})
  endforeach(ITR)

  # platform specific flags
  include("cmake/${PLATFORM}/flags.cmake")

  # layout
  set(PLATFORM_LAYOUT "cosmos.ld")

  # Export our output variables
  set(PLATFORM_SRCS
      ${PLATFORM_SRCS}
      PARENT_SCOPE
  )
  set(PLATFORM_LAYOUT
      ${PLATFORM_LAYOUT}
      PARENT_SCOPE
  )
  set(ALL_PLATFORM_SRCS
      ${ALL_PLATFORM_SRCS}
      PARENT_SCOPE
  )
  set(ISA_C_FLAGS
      ${ISA_C_FLAGS}
      PARENT_SCOPE
  )
  set(ISA_ASM_FLAGS
      ${ISA_ASM_FLAGS}
      PARENT_SCOPE
  )
endfunction(LOAD_PLATFORM)
