#ifndef STACK_H
#define STACK_H

#define DECLARE_STACK(TYPE) \
    struct TYPE ## _stack_item { \
        TYPE data; \
        struct TYPE ## _stack_item *next; \
    }; \
    struct TYPE ## _stack { \
        struct TYPE ## _stack_item *top; \
    }; \
    void TYPE ## _stack_init(struct TYPE ## _stack *stack); \
    void TYPE ## _stack_destroy(struct TYPE ## _stack *stack); \
    void TYPE ## _stack_push(TYPE data, struct TYPE ## _stack *stack); \
    int TYPE ## _stack_empty(const struct TYPE ## _stack *stack); \
    TYPE TYPE ## _stack_pop(struct TYPE ## _stack *stack);

DECLARE_STACK(int)
DECLARE_STACK(char)

#ifdef DEBUG
#   include <stdio.h>
#   define DECLARE_STACK_PRINT(TYPE) \
        void TYPE ## _stack_print(FILE *stream, \
                const struct TYPE ## _stack *stack);

    DECLARE_STACK_PRINT(int)
    DECLARE_STACK_PRINT(char)
#endif

#endif
