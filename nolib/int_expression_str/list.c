#include <stdlib.h>

#include "list.h"


static int str_list_empty(const struct str_list *list)
{
    return list->first == NULL;
}


void str_list_init(struct str_list *list)
{
    list->first = NULL;
    list->last = NULL;
}


void str_list_del(struct str_list *list)
{
    while (!str_list_empty(list)) {
        str_list_remove_front(list);
    }
}


void str_list_push_back(struct str_list *list)
{
    struct str_list_item *tmp = malloc(sizeof(struct str_list_item));
    tmp->next = NULL;
    str_init(&tmp->s);
    if (list->last) {
        list->last->next = tmp;
    } else {
        list->first = tmp;
    }
    list->last = tmp;
}


void str_list_remove_front(struct str_list *list)
{
    struct str_list_item *tmp = list->first;
    list->first = tmp->next;
    if (str_list_empty(list)) {
        list->last = NULL;
    }
    str_del(&tmp->s);
    free(tmp);
}
