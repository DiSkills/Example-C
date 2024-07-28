#ifndef STR_LIST_H
#define STR_LIST_H

#include "string.h"

struct str_list_item {
    struct string *str;

    struct str_list_item *next;
};

struct str_list {
    struct str_list_item *head;
};

struct str_list *str_list_init();

void str_list_push_front(struct str_list *list);

void str_list_print(unsigned char separator, const struct str_list *list);

void str_list_del(struct str_list **plist);

#endif
