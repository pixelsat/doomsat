#include <stdio.h>

FILE _stdout = { 254 };
FILE _stderr = { 255 };

FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

int putchar(int ch) {
        DG_putc_stdout(ch);
    return ch;
}

int putc(int ch, FILE *stream) {
    if (stream->type == 254) {
        DG_putc_stdout(ch);
    } else {
        DG_putc_stderr(ch);
    }
    return ch;
}

int puts(char *s) {
    while (*s) {
        if (putchar(*s++) == EOF)
            return EOF;
    }
    return putchar('\n') == EOF ? EOF : 0;
}

int fflush(FILE *file) {
    if (file->type == 254) {
        DG_flush_stdout();
    } else {
        DG_flush_stderr();
    }
    return 0;
}

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_ALT_FORM_FLAG 1

#define NANOPRINTF_USE_FLOAT_SCI_FORMAT_SPECIFIER 0
#define NANOPRINTF_USE_FLOAT_SHORTEST_FORMAT_SPECIFIER 0
#define NANOPRINTF_USE_FLOAT_HEX_FORMAT_SPECIFIER 0

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

void doomsat_putc(int c, void *ctx) {
    putc(c, (FILE *)ctx);
}

int printf(char *format, ...) {
    va_list val;
    va_start(val, format);
    int const rv = npf_vpprintf(&doomsat_putc, (void *) &stdout, format, val);
    va_end(val);
    return rv;
}
int fprintf(FILE *file, char *format, ...) {
    va_list val;
    va_start(val, format);
    int const rv = npf_vpprintf(&doomsat_putc, (void *) file, format, val);
    va_end(val);
    return rv;
}
int snprintf(char *buf, size_t n, const char *format, ...) {
    va_list val;
    va_start(val, format);
    int const rv = npf_vsnprintf(buf, n, format, val);
    va_end(val);
    return rv;
}
int vfprintf(FILE *file, char *format, va_list val) {
    npf_vpprintf(&doomsat_putc, (void *) file, format, val);
}
int vsnprintf(char *buf, size_t n, const char *format, va_list val) {
    int const rv = npf_vsnprintf(buf, n, format, val);
    return rv;
}