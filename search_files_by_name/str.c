#include <stdlib.h>
#include <string.h>

#include "str.h"


enum {
    min_str_size = 16
};


void str_init(struct str_t *str)
{
    str->data = malloc(min_str_size);
    str->size = min_str_size;
    str_clear(str);
}


void str_del(struct str_t *str)
{
    free(str->data);
    str->len = 0;
    str->size = 0;
    str->data = NULL;
}


void str_clear(struct str_t *str)
{
    str->data[0] = '\0';
    str->len = 0;
}


static int str_is_full(const struct str_t *str)
{
    return str->len + 1 == str->size;
}


static void str_resize(struct str_t *str)
{
    str->size *= 2;
    str->data = realloc(str->data, str->size);
}


void str_append(struct str_t *str, char c)
{
    if (str_is_full(str)) {
        str_resize(str);
    }
    str->data[str->len] = c;
    str->len++;
    str->data[str->len] = '\0';
}
