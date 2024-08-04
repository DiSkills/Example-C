#ifndef LIST_H
#define LIST_H

#include "str.h"

struct str_list_item {
    struct str s;
    struct str_list_item *next;
};

struct str_list {
    struct str_list_item *first,
                         *last;
};

void str_list_init(struct str_list *list);
void str_list_del(struct str_list *list);
void str_list_push_back(struct str_list *list);
void str_list_remove_front(struct str_list *list);

#endif
