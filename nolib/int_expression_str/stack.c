#include <stdlib.h>

#include "stack.h"

#define DEFINE_STACK(TYPE) \
    void TYPE ## _stack_init(struct TYPE ## _stack *stack) \
    { \
        stack->top = NULL; \
    } \
    void TYPE ## _stack_destroy(struct TYPE ## _stack *stack) \
    { \
        while (!TYPE ## _stack_empty(stack)) { \
            TYPE ## _stack_pop(stack); \
        } \
    } \
    void TYPE ## _stack_push(TYPE data, struct TYPE ## _stack *stack) \
    { \
        struct TYPE ## _stack_item *tmp = malloc(sizeof(struct TYPE ## _stack_item)); \
        tmp->data = data; \
        tmp->next = stack->top; \
        stack->top = tmp; \
    } \
    int TYPE ## _stack_empty(const struct TYPE ## _stack *stack) \
    { \
        return stack->top == NULL; \
    } \
    TYPE TYPE ## _stack_pop(struct TYPE ## _stack *stack) \
    { \
        struct TYPE ## _stack_item *tmp = stack->top; \
        TYPE data = tmp->data; \
        stack->top = tmp->next; \
        free(tmp); \
        return data; \
    }


DEFINE_STACK(int)
DEFINE_STACK(char)
