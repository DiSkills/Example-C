#include <stdio.h>
#include <stdlib.h>

#include "int_list.h"


struct item *int_array_to_list(const int *array, int len)
{
    int i;
    struct item *head = NULL;

    for (i = len - 1; i >= 0; i--) {
        struct item *tmp = malloc(sizeof(struct item));
        tmp->data = array[i];
        tmp->next = head;
        head = tmp;
    }

    return head;
}


static int length_of_int_list(const struct item *list)
{
    int len = 0;
    for (; list; list = list->next) {
        len++;
    }
    return len;
}


int *int_list_to_array(const struct item *list)
{
    int i,
        len,
        *array;

    len = length_of_int_list(list) + 1;

    array = malloc(sizeof(int) * len);
    array[0] = len - 1;

    for (i = 1; i < len; i++) {
        array[i] = list->data;
        list = list->next;
    }
    return array;
}


void delete_int_list(struct item **plist)
{
    while (*plist) {
        struct item *tmp = *plist;
        *plist = (*plist)->next;
        free(tmp);
    }
}


void print_int_list(const struct item *list)
{
    for (; list; list = list->next) {
        printf("%d ", list->data);
    }
    putchar('\n');
}
