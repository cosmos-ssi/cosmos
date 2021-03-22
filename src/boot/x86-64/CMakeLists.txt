set(CAN_USE_ASSEMBLER TRUE)
set(CMAKE_ASM_NASM_OBJECT_FORMAT bin)
enable_language(ASM_NASM)

add_compile_options(-O0)

add_library(boot STATIC boot.asm)
add_library(boot2 STATIC boot2.asm)
add_library(boot3 STATIC boot3.asm)
