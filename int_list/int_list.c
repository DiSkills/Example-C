#include <stdio.h>
#include <stdlib.h>

#include "int_list.h"


struct int_list *int_array_to_list(const int *array, int len)
{
    int i;
    struct int_list *list = malloc(sizeof(struct int_list));

    list->size = len;
    list->head = NULL;
    list->tail = NULL;
    for (i = 0; i < len; i++) {
        struct item *tmp = malloc(sizeof(struct item));
        tmp->data = array[i];
        tmp->next = NULL;

        if (list->tail) {
            list->tail->next = tmp;
        } else {
            list->head = tmp;
        }
        list->tail = tmp;
    }

    return list;
}


int *int_list_to_array(const struct int_list *list)
{
    int len,
        *array;
    const struct item *p;

    len = list->size + 1;
    array = malloc(len * sizeof(int));

    *array = list->size;
    array++;
    for (p = list->head; p; p = p->next) {
        *array = p->data;
        array++;
    }
    return array - len;
}


void delete_int_list(struct int_list **plist)
{
    while ((*plist)->head) {
        struct item *tmp = (*plist)->head;
        (*plist)->head = (*plist)->head->next;
        free(tmp);
    }
    free(*plist);
    *plist = NULL;
}


void print_int_list(const struct int_list *list)
{
    const struct item *p;
    for (p = list->head; p; p = p->next) {
        printf("%d ", p->data);
    }
    putchar('\n');
}
