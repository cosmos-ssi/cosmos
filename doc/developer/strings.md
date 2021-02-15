# Cosmos Strings

Looking at the header file [`string.h`](../../src/kernel/sys/string/string.h) you will likely notice that `strcpy`, and `strcat` don't exist, but instead there are `strncpy` and `strncat`. We've chosen to use API's that know the size of the destination buffer, in order to reduce the likelihood of accidental memory overruns.
