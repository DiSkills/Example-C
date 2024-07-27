#include <stdio.h>
#include <stdlib.h>

#include "int_list.h"

int main()
{
    int i,
        size,
        *numbers,
        array[] = { 1, 3, 5, 7, 9, 11, 13 };
    struct int_list *list;

    size = sizeof(array) / sizeof(*array);

    list = int_array_to_list(array, size);
    puts("List output:");
    print_int_list(list);

    numbers = int_list_to_array(list);
    puts("Array output:");
    for (i = 0; i < size + 1; i++) {
        printf("%d ", numbers[i]);
    }
    putchar('\n');

    delete_int_list(&list);
    free(numbers);
    return 0;
}
