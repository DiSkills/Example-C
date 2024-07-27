#ifndef INT_LIST_H
#define INT_LIST_H

struct item {
    int data;
    struct item *next;
};

struct int_list {
    unsigned size;

    struct item *head,
                *tail;
};

struct int_list *int_array_to_list(const int *array, int len);

int *int_list_to_array(const struct int_list *list);

void delete_int_list(struct int_list **plist);

void print_int_list(const struct int_list *list);

#endif
