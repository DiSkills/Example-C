#include <stdlib.h>

#include "strings.h"


int string_length(const char *str)
{
    const char *p;
    for (p = str; *p; p++)
        {}
    return p - str;
}


void string_copy(char *dest, const char *src)
{
    for (; *src; src++, dest++) {
        *dest = *src;
    }
    *dest = '\0';
}


void string_n_copy(char *dest, const char *src, int n)
{
    for (; *src && n; src++, dest++, n--) {
        *dest = *src;
    }
    for (; n; dest++, n--) {
        *dest = '\0';
    }
}


char *strdup(const char *s)
{
    char *dest = malloc((1 + string_length(s)) * sizeof(char));
    string_copy(dest, s);
    return dest;
}


int string_compare(const char *s1, const char *s2)
{
    for (; *s1 && *s2 && *s1 == *s2; s1++, s2++)
        {}
    return *s1 - *s2;
}


int string_n_compare(const char *s1, const char *s2, int n)
{
    for (; *s1 && *s2 && *s1 == *s2 && n; s1++, s2++, n--)
        {}
    return !n ? 0 : *s1 - *s2;
}


char *string_char(const char *s, int c)
{
    for (; *s && *s != c; s++)
        {}
    return *s ? (char*)s : NULL;
}


char *string_r_char(const char *s, int c)
{
    const char *p = NULL;
    for (; *s; s++) {
        if (*s == c) {
            p = s;
        }
    }
    return (char*)p;
}
