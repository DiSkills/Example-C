#include <stdio.h>
#include <stdlib.h>

#include "string.h"


struct string *string_init()
{
    struct string *str = malloc(sizeof(struct string));
    str->size = 0;
    str->head = NULL;
    str->tail = NULL;
    return str;
}


void string_append(unsigned char data, struct string *str)
{
    struct char_item *tmp = malloc(sizeof(struct char_item));
    tmp->data = data;
    tmp->next = NULL;

    str->size++;
    if (str->tail) {
        str->tail->next = tmp;
    } else {
        str->head = tmp;
    }
    str->tail = tmp;
}


void string_print(const struct string *str)
{
    const struct char_item *p;
    for (p = str->head; p; p = p->next) {
        putchar(p->data);
    }
}


void string_del(struct string **pstr)
{
    while ((*pstr)->head) {
        struct char_item *tmp = (*pstr)->head;
        (*pstr)->head = (*pstr)->head->next;
        free(tmp);
    }
    free(*pstr);
    *pstr = NULL;
}
