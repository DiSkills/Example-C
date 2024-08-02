#ifndef STRINGS_H
#define STRINGS_H

int string_length(const char *str);

void string_copy(char *dest, const char *src);

void string_n_copy(char *dest, const char *src, int n);

char *strdup(const char *s);

int string_compare(const char *s1, const char *s2);

int string_n_compare(const char *s1, const char *s2, int n);

char *string_char(const char *s, int c);

char *string_r_char(const char *s, int c);

#endif
