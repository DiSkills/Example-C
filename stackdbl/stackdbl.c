#include <stdlib.h>

#include "stackdbl.h"


enum {
    stackdbl_min_size = 16
};


struct stackdbl *stackdbl_init()
{
    struct stackdbl *stack = malloc(sizeof(struct stackdbl));
    stack->size = 0;
    stack->max_size = stackdbl_min_size;
    stack->elements = malloc(stackdbl_min_size * sizeof(double));
    return stack;
}


void stackdbl_destroy(struct stackdbl **pstack)
{
    free((*pstack)->elements);
    free(*pstack);
    *pstack = NULL;
}


int stackdbl_empty(const struct stackdbl *stack)
{
    return stack->size == 0;
}


static int stackdbl_full(const struct stackdbl *stack)
{
    return stack->size == stack->max_size;
}


static void stackdbl_resize(struct stackdbl *stack)
{
    int i;
    double *tmp = stack->elements;

    stack->max_size *= 2;
    stack->elements = malloc(stack->max_size * sizeof(double));
    for (i = 0; i < stack->size; i++) {
        stack->elements[i] = tmp[i];
    }

    free(tmp);
}


void stackdbl_push(double data, struct stackdbl *stack)
{
    if (stackdbl_full(stack)) {
        stackdbl_resize(stack);
    }
    stack->elements[stack->size] = data;
    stack->size++;
}


double stackdbl_pop(struct stackdbl *stack)
{
    stack->size--;
    return stack->elements[stack->size];
}
