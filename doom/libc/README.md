doomsat's implementation of a subset of libc. we stand on the shoulders of giants!

DO NOT create these files as they are maintained by clang regardless of target (and would get shadowed if we were to create them):
* stddef.h
* stdint.h
* stdarg.h
* stdbool.h
* limits.h
* float.h

many implementations (especially for the "simple" functions like string helpers) are vendored from picolibc [0], while others are either implemented specifically how they are needed for doomsat to work, and others are vendored from picolib then patched to call doomsat internals.

printf and variants are vendored from nanoprintf [1].

[0]: https://github.com/picolibc/picolibc
[1]: https://github.com/charlesnicholson/nanoprintf