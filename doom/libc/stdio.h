#include <stdarg.h>
#include <stdint.h>
#define __need_size_t
#define __need_NULL
#include <stddef.h>
#ifndef DOOMSAT_STDIO
#define DOOMSAT_STDIO

#define EOF -1
struct __FILE
{
    uint8_t type; // 254 = stdout, 255 = stderr
};

typedef struct __FILE FILE;

extern FILE *stdout;
extern FILE *stderr;

void DG_putc_stdout (int ch);
void DG_putc_stderr (int ch);
void DG_flush_stdout (void);
void DG_flush_stderr (void);

int putchar (int ch);
int putc (int ch, FILE *stream);
int puts (char *s);
int printf (char *format, ...);
int fprintf (FILE *file, char *format, ...);
int snprintf (char *buf, size_t n, const char *format, ...);
int vfprintf (FILE *file, char *format, va_list val);
int vsnprintf (char *buf, size_t n, const char *format, va_list val);
int fflush (FILE *file);

#endif