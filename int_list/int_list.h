#ifndef INT_LIST_H
#define INT_LIST_H

struct item {
    int data;
    struct item *next;
};

struct item *int_array_to_list(const int *array, int len);

int *int_list_to_array(const struct item *list);

void delete_int_list(struct item **plist);

void print_int_list(const struct item *list);

#endif
