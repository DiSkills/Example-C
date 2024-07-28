#include <stdio.h>

#include "stackdbl.h"


int main()
{
    int i,
        n = 16;
    struct stackdbl *stack = stackdbl_init();

    for (i = 0; i < n; i++) {
        stackdbl_push((double)i / 10, stack);
        stackdbl_push((double)i, stack);
    }

    for (i = 0; i < n; i++) {
        printf("%f ", stackdbl_pop(stack));
    }
    putchar('\n');

    stackdbl_destroy(&stack);
    return 0;
}
