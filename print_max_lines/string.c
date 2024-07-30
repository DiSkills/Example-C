#include <stdlib.h>

#include "string.h"


enum {
    string_min_size = 16
};


struct string *string_init()
{
    struct string *str = malloc(sizeof(struct string));
    str->symbols = malloc(string_min_size * sizeof(unsigned char));

    *str->symbols = '\0';
    str->size = 0;
    str->max_size = string_min_size;
    return str;
}


void string_del(struct string **pstr)
{
    free((*pstr)->symbols);
    free(*pstr);
    *pstr = NULL;
}


static int string_full(const struct string *str)
{
    return str->size + 1 == str->max_size;
}


static void string_resize(struct string *str)
{
    int i;
    unsigned char *tmp = str->symbols;

    str->max_size *= 2;
    str->symbols = malloc(str->max_size * sizeof(unsigned char));
    for (i = 0; i <= str->size; i++) {
        str->symbols[i] = tmp[i];
    }

    free(tmp);
}


void string_append(unsigned char ch, struct string *str)
{
    if (string_full(str)) {
        string_resize(str);
    }
    str->symbols[str->size] = ch;
    str->size++;
    str->symbols[str->size] = '\0';
}
