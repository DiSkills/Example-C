#ifndef STACK_H
#define STACK_H

struct int_stack_item {
    int data;
    struct int_stack_item *next;
};

struct int_stack {
    struct int_stack_item *top;
};

void int_stack_init(struct int_stack *stack);
void int_stack_destroy(struct int_stack *stack);
void int_stack_push(int data, struct int_stack *stack);
int int_stack_empty(const struct int_stack *stack);
int int_stack_pop(struct int_stack *stack);

#endif
