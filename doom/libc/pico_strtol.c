#include <ctype.h>
#define __need_NULL
#include <stddef.h>

// local-stdio.h
#include <limits.h>
#define TOLOWER(c) ((c) | ('a' - 'A'))

// strtol.c
#define strtoi_type  long
#define strtoi_utype unsigned long
#define strtoi_max   LONG_MAX
#define strtoi_min   LONG_MIN
#define strtoi_signed
#ifdef WIDE_CHARS
#define strtoi wcstol
#else
#define strtoi strtol
#endif

static inline unsigned int
digit_to_val(unsigned int c)
{
    /*
     * Convert letters with some tricky code.
     *
     * TOLOWER(c-1) maps characters as follows (Skipping values not
     * greater than '9' (0x39), as those are skipped by the 'if'):
     *
     * Minus 1, bitwise-OR ('a' - 'A') (0x20):
     *
     *             0x3a..0x40 -> 0x39..0x3f
     * 0x41..0x60, 0x61..0x80 -> 0x60..0x7f
     * 0x81..0xa0, 0xa1..0xc0 -> 0xa0..0xbf
     * 0xc1..0xe0, 0xe1..0x00 -> 0xe0..0xff
     *
     * Plus '0' (0x30), minus 'a') (0x61), plus 11 (0xb), for
     * a total of minus 0x26:
     *
     *             0x3a..0x40 -> 0x39..0x3f -> 0x13..0x19
     * 0x41..0x60, 0x61..0x80 -> 0x60..0x7f -> 0x3a..0x59
     * 0x81..0xa0, 0xa1..0xc0 -> 0xa0..0xbf -> 0x7a..0x99
     * 0xc1..0xe0, 0xe1..0x00 -> 0xe0..0xff -> 0xba..0xd9
     */

    if (c > '9') {

        /*
         * For the letters, we want TOLOWER(c) - 'a' + 10, but that
         * would map both '@' and '`' to 9.
         *
         * To work around this, subtract 1 before the bitwise-or so
         * that '@' (0x40) gets mapped down to 0x3f (0x3f | 0x20)
         * while '`' (0x60) gets mapped up to 0x7f (0x5f | 0x20),
         * moving them away from the letters (which end up in the
         * range 0x60..0x79). Then add the 1 back in when subtracting
         * 'a' and adding 10.
         *
         * Add in '0' so that it can get subtracted out in the common
         * code (c -= '0') below, avoiding an else clause.
         */

        c = TOLOWER(c - 1) + ('0' - 'a' + 11);
    }

    /*
     * Now, include the range from NUL (0x00) through '9' (0x39)
     *
     * Minus '0' (0x30):
     *
     * 0x00..0x2f                                         ->-0x30..-0x01
     * 0x30..0x39                                         -> 0x00..0x09 *
     *             0x3a..0x40 -> 0x39..0x3f -> 0x13..0x19 ->-0x1d..-0x17
     * 0x41..0x60, 0x61..0x80 -> 0x60..0x7f -> 0x3a..0x59 -> 0x0a..0x29 *
     * 0x81..0xa0, 0xa1..0xc0 -> 0xa0..0xbf -> 0x7a..0x99 -> 0x4a..0x69
     * 0xc1..0xe0, 0xe1..0x00 -> 0xe0..0xff -> 0xba..0xd9 -> 0x8a..0xa9
     *
     * The first starred row has the digits '0'..'9', while the second
     * starts with the letters 'A'..'Z' and 'a'..'z'. All of the other
     * rows end up with values above any allowed conversion base
     */

    c -= '0';
    return c;
}

// strtoi.h
#ifdef WIDE_CHARS
#include <wctype.h>
#include <wchar.h>
#define strtoi_char wchar_t
#if __SIZEOF_WCHAR_T__ == 2
#define strtoi_uchar uint16_t
#elif __SIZEOF_WCHAR_T__ == 4
#define strtoi_uchar uint32_t
#endif
#define strtoi_uint       strtoi_uchar
#define strtoi_isspace(c) iswspace(c)
#else
#define strtoi_char       char
#define strtoi_uint       unsigned int
#define strtoi_uchar      unsigned char
#define strtoi_isspace(c) isspace(c)
#endif

#ifndef strtoi_signed
#define strtoi_utype strtoi_type
#endif

#if __HAVE_BUILTIN_MUL_OVERFLOW && __HAVE_BUILTIN_ADD_OVERFLOW && !defined(strtoi_signed)
#define USE_OVERFLOW
#endif

strtoi_type
strtoi(const strtoi_char * __restrict nptr, strtoi_char ** __restrict endptr, int ibase)
{
    unsigned int base = ibase;

    /* Check for invalid base value */
    if (base > 36 || base == 1) {
        if (endptr)
            *endptr = (strtoi_char *)nptr;
        return 0;
    }

#define FLAG_NEG   0x1 /* Negative. Must be 1 for ucutoff below */
#define FLAG_OFLOW 0x2 /* Value overflow */

    const strtoi_uchar *s = (const strtoi_uchar *)nptr;
    strtoi_utype        val = 0;
    unsigned char       flags = 0;
    strtoi_uint         i;

    /* Skip leading spaces */
    do {
        i = *s++;
    } while (strtoi_isspace(i));

    /* Parse a leading sign */
    switch (i) {
    case '-':
        flags = FLAG_NEG;
        [[fallthrough]];
    case '+':
        i = *s++;
    }

    /* Leading '0' digit -- check for base indication */
    if (i == '0') {
        if (TOLOWER(*s) == 'x' && ((base | 16) == 16)) {
            base = 16;
            /* Parsed the '0' */
            nptr = (const strtoi_char *)s;
            i = s[1];
            s += 2;
        } else if (TOLOWER(*s) == 'b' && ((base | 2) == 2)) {
            base = 2;
            /* Parsed the '0' */
            nptr = (const strtoi_char *)s;
            i = s[1];
            s += 2;
        } else if (base == 0) {
            base = 8;
        }
    } else if (base == 0) {
        base = 10;
    }

#ifndef USE_OVERFLOW
    /* Compute values used to detect overflow. */
#ifdef strtoi_signed
    /* works because strtoi_min = (strtoi_type) ((strtoi_utype) strtoi_max + 1) */
    strtoi_utype ucutoff = (strtoi_utype)strtoi_max + flags;
    strtoi_utype cutoff = ucutoff / base;
    unsigned int cutlim = ucutoff % base;
#else
    strtoi_type  cutoff = strtoi_max / base;
    unsigned int cutlim = strtoi_max % base;
#endif
#endif

    for (;;) {
        i = digit_to_val(i);
        /* detect invalid char */
        if (i >= base)
            break;

        /* Add the new digit, checking for overflow */
#ifdef USE_OVERFLOW
        /*
         * This isn't used for signed values as it's tricky and
         * generates larger code. Yes, it avoids doing the divmod
         * above, but we'll assume an app doing math with signed
         * values will probably end up doing a divide somewhere
         */
        if (__builtin_mul_overflow(val, (strtoi_type)base, &val)
            || __builtin_add_overflow(val, (strtoi_type)i, &val)) {
            flags |= FLAG_OFLOW;
        }
#else
        if (val > cutoff || (val == cutoff && i > cutlim))
            flags |= FLAG_OFLOW;
        else
            val = val * (strtoi_utype)base + (strtoi_utype)i;
#endif
        /* Parsed another digit */
        nptr = (const strtoi_char *)s;
        i = *s++;
    }

    /* Mark the end of the parsed region */
    if (endptr != NULL)
        *endptr = (strtoi_char *)nptr;

    if (flags & FLAG_NEG)
        val = -val;

    if (flags & FLAG_OFLOW) {
#ifdef strtoi_signed
        val = ucutoff;
#else
        val = strtoi_max;
#endif
    }

    return (strtoi_type)val;
}