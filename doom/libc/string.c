#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int
islower (int c)
{
    return 'a' <= c && c <= 'z';
}
int
isupper (int c)
{
    return 'A' <= c && c <= 'Z';
}
int
toupper (int c)
{
    return islower (c) ? c - 'a' + 'A' : c;
}
int
tolower (int c)
{
    if (isupper (c))
        c = c - 'A' + 'a';
    return c;
}

int
strcasecmp (const char *s1, const char *s2)
{
    int d = 0;
    for (;;)
        {
            const int c1 = tolower (*(unsigned char *)s1++);
            const int c2 = tolower (*(unsigned char *)s2++);
            if (((d = c1 - c2) != 0) || (c2 == '\0'))
                break;
        }
    return d;
}

int
strncasecmp (const char *s1, const char *s2, size_t n)
{
    int d = 0;
    for (; n != 0; n--)
        {
            const int c1 = tolower (*(unsigned char *)s1++);
            const int c2 = tolower (*(unsigned char *)s2++);
            if (((d = c1 - c2) != 0) || (c2 == '\0'))
                break;
        }
    return d;
}

int
strcmp (const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2)
        {
            s1++;
            s2++;
        }

    return (*(unsigned char *)s1) - (*(unsigned char *)s2);
}
int
strncmp (const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    while (n-- != 0 && *s1 == *s2)
        {
            if (n == 0 || *s1 == '\0')
                break;
            s1++;
            s2++;
        }

    return (*(unsigned char *)s1) - (*(unsigned char *)s2);
}

char *
strdup (const char *str)
{
    size_t len = strlen (str) + 1;
    char *copy = malloc (len);
    if (copy)
        {
            memcpy (copy, str, len);
        }
    return copy;
}

char *
strchr (const char *s1, int i)
{
    const unsigned char *s = (const unsigned char *)s1;
    unsigned char c = i;

    while (*s && *s != c)
        s++;
    if (*s == c)
        return (char *)s;
    return NULL;
}

char *
strrchr (const char *s, int i)
{
    const char *last = NULL;
    char c = i;

    if (c)
        {
            while ((s = strchr (s, c)))
                {
                    last = s;
                    s++;
                }
        }
    else
        {
            last = strchr (s, c);
        }

    return (char *)last;
}

char *
strncpy (char *dst0, const char *src0, size_t count)
{
    char *dscan;
    const char *sscan;

    dscan = dst0;
    sscan = src0;
    while (count > 0)
        {
            --count;
            if ((*dscan++ = *sscan++) == '\0')
                break;
        }
    while (count-- > 0)
        *dscan++ = '\0';

    return dst0;
}

char *
strstr (const char *hs, const char *ne)
{
    size_t i;
    int c = ne[0];

    if (c == 0)
        return (char *)hs;

    for (; hs[0] != '\0'; hs++)
        {
            if (hs[0] != c)
                continue;
            for (i = 1; ne[i] != 0; i++)
                if (hs[i] != ne[i])
                    break;
            if (ne[i] == '\0')
                return (char *)hs;
        }

    return NULL;
}