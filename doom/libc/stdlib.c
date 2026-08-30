#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "z_zone.h"

void *
_doomsat_malloc (size_t size, char *file, int line)
{
    if (size > 10 * 1024) { printf("[%s:%d] mallocating %d bytes\n", file, line, size); }

    if (size > INT_MAX)
        return NULL;

    return Z_Malloc ((int)size, PU_STATIC, NULL);
}

void
doomsat_free (void *ptr)
{
    if (ptr != NULL)
        Z_Free (ptr);
}

void *
doomsat_calloc (size_t count, size_t size)
{
    size_t total;
    void *ptr;

    if (size != 0 && count > (size_t)-1 / size)
        return NULL;

    total = count * size;
    ptr = malloc (total);
    if (ptr != NULL)
        memset (ptr, 0, total);

    return ptr;
}

void *
doomsat_realloc (void *ptr, size_t size)
{
    size_t old_size;
    void *new_ptr;

    if (ptr == NULL)
        return malloc (size);

    if (size == 0)
        {
            free (ptr);
            return NULL;
        }

    old_size = Z_AllocationSize (ptr);
    if (size <= old_size)
        return ptr;

    new_ptr = malloc (size);
    if (new_ptr == NULL)
        return NULL;

    memcpy (new_ptr, ptr, old_size);
    free (ptr);
    return new_ptr;
}

int
abs (int i)
{
    return (i < 0) ? -i : i;
}

double
fabs (double x)
{
    return x < 0.0 ? -x : x;
}

int
atoi (const char *s)
{
    return (int)strtol (s, NULL, 10);
}