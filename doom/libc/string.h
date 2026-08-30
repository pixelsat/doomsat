#ifndef DOOMSAT_STRING_H
#define DOOMSAT_STRING_H

#define __need_size_t
#define __need_NULL
#include <stddef.h>

// all externs are declared by rust's compiler_builtins[0], which is part of
// core and thus will be available to link into even in our no_std builds
// [0]:
// https://docs.rs/compiler_builtins/latest/compiler_builtins/mem/index.html

extern void *memcpy (void *dest, const void *src, size_t n);

extern void *memset (void *s, int c, size_t n);

extern size_t strlen (const char *str);

void *memmove (void *dest, void *src, size_t n);

int toupper (int c);

int strcasecmp (const char *s1, const char *s2);

int strncasecmp (const char *s1, const char *s2, size_t n);

int strcmp (const char *s1, const char *s2);

int strncmp (const char *s1, const char *s2, size_t n);

char *strdup (const char *str);

char *strrchr (const char *s, int i);

char *strncpy (char *dst0, const char *src0, size_t count);

char *strstr (const char *hs, const char *ne);

#endif
