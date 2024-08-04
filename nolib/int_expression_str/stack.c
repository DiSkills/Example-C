#include <stdlib.h>

#include "stack.h"


void int_stack_init(struct int_stack *stack)
{
    stack->top = NULL;
}


void int_stack_destroy(struct int_stack *stack)
{
    while (!int_stack_empty(stack)) {
        int_stack_pop(stack);
    }
}


void int_stack_push(int data, struct int_stack *stack)
{
    struct int_stack_item *tmp = malloc(sizeof(struct int_stack_item));
    tmp->data = data;
    tmp->next = stack->top;
    stack->top = tmp;
}


int int_stack_empty(const struct int_stack *stack)
{
    return stack->top == NULL;
}


int int_stack_pop(struct int_stack *stack)
{
    struct int_stack_item *tmp = stack->top;
    int data = tmp->data;
    stack->top = tmp->next;
    free(tmp);
    return data;
}
