function(LOAD_PLATFORM_SOURCES PLATFORM)
  # enum of available target platforms
  list(
    APPEND
    PLATFORMS
    "x86_64"
    "arm"
  )

  # search for target platform-specific files
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

  # search for platform-specific files for all platforms except our target
  list(REMOVE_ITEM PLATFORMS ${PLATFORM}})
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

  # Export our output variables
  set(PLATFORM_SRCS
      ${PLATFORM_SRCS}
      PARENT_SCOPE
  )
  set(_IGNORE
      ${_IGNORE}
      PARENT_SCOPE
  )
endfunction(LOAD_PLATFORM_SOURCES)
