#define __need_size_t
#define __need_NULL
#include <stddef.h>

int abs (int i);
double fabs (double x);
int strcmp (const char *s1, const char *s2);

long strtol (char *nptr, char **endptr, int ibase);
unsigned long strtoul (char *nptr, char **endptr, int ibase);

int atoi (const char *s);

// TODO(doomsat): implement lmao
void *malloc (size_t size);

void *doomsat_malloc (size_t size);
void doomsat_free (void *ptr);
void *doomsat_calloc (size_t count, size_t size);
void *doomsat_realloc (void *ptr, size_t size);

#define malloc doomsat_malloc
#define free doomsat_free
#define calloc doomsat_calloc
#define realloc doomsat_realloc