# CosmOS coding standards

- Standard based on LLVM and defined in `.clang-format` in project root. Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) as your code formatter, and `make lint` to check for errors.
- CosmOS is spelled <i>CosmOS</i>
- CosmOS uses [Snake Case](https://en.wikipedia.org/wiki/Snake_case) for function names
- Do not include header files from `/kernel/dev/`. Instead, query the DeviceMgr for the appropriate device and use the abstract device API from `/sys/deviceapi/`

# Configure your editor for code formatting

## VSCode

Configure VSCode to use the `cLangFormat` formatter, and `file` for style options, which will refer to `.clang-format`.

In `settings.json`:

```json5
{
  // Coding style. Use "file" to load the style from a .clang-format file in the current or parent directory.
  'C_Cpp.clang_format_style': 'file',

  // Configures the formatting engine for clangFormat.
  'C_Cpp.formatting': 'clangFormat',

  // Format a file on save. Formatter must be available.
  'editor.formatOnSave': true,
}
```

# include-what-you-use

As discussed in [IWYU](https://include-what-you-use.org/), the main goal of include-what-you-use is to remove superfluous #includes. It does this both by figuring out what #includes are not actually needed for this file (for both .c and .h files), and replacing #includes with forward-declares when possible.

To scan for problems:
`make header-check`

To automatically fix problems (careful, you should commit first in case this goes bad):
`make header-fix`

IWYU is built on Make, which means it only acts on files which have changed. Since some code changes can impact headers in other files, it's a best practice to run `make clean; make header-check` before you commit.

## IWYU Mappings

IWYU uses hard-coded defaults for common libc symbols, which we need to override manually for a freestanding build. This is done via the `iwyu_mapping.imp` file,
and you can learn the format [here](https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUMappings.md).

## Idiosyncrasies

IWYU is alpha quality, built on top of clang, by and for Google, mostly for C++ codebases, and with no attention paid to freestanding requirements. Making it work in an osdev context is difficult and error prone.

You will find it recommending `stdlib` based on your system headers from time to time, which you can fix by providing the correct headers and/or adding a mapping. This is most likely to happen with common types & includes like `uint8_t` or `<types.h>`. Most of the most obvious issues have been worked around, but YMMV.
