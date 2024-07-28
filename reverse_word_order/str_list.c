#include <stdio.h>
#include <stdlib.h>

#include "str_list.h"


struct str_list *str_list_init()
{
    struct str_list *list = malloc(sizeof(struct str_list));
    list->head = NULL;
    return list;
}


void str_list_push_front(struct str_list *list)
{
    struct str_list_item *tmp = malloc(sizeof(struct str_list_item));
    tmp->str = string_init();
    tmp->next = list->head;
    list->head = tmp;
}


void str_list_print(unsigned char separator, const struct str_list *list)
{
    const struct str_list_item *p;
    for (p = list->head; p; p = p->next) {
        string_print(p->str);
        putchar(p->next ? separator : '\n');
    }
}


void str_list_del(struct str_list **plist)
{
    while ((*plist)->head) {
        struct str_list_item *tmp = (*plist)->head;
        string_del(&tmp->str);
        (*plist)->head = (*plist)->head->next;
        free(tmp);
    }
    free(*plist);
    *plist = NULL;
}
