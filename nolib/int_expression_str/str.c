#include <stdlib.h>
#include <string.h>

#include "str.h"


enum {
    str_min_size = 16
};


void str_init(struct str *s)
{
    s->items = malloc(str_min_size * sizeof(char));
    s->items[0] = '\0';
    s->size = 1;
    s->max_size = str_min_size;
}


void str_del(struct str *s)
{
    s->size = 0;
    s->max_size = 0;
    free(s->items);
    s->items = NULL;
}


static int str_full(const struct str *s)
{
    return s->size == s->max_size;
}


static void str_resize(struct str *s)
{
    char *tmp = s->items;

    s->max_size *= 2;
    s->items = malloc(s->max_size * sizeof(char));
    strcpy(s->items, tmp);

    free(tmp);
}


void str_append(char ch, struct str *s)
{
    if (str_full(s)) {
        str_resize(s);
    }
    s->items[s->size - 1] = ch;
    s->items[s->size] = '\0';
    s->size++;
}
