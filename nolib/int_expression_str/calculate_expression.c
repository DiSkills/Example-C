#include "stack.h"

#include "calculate_expression.h"


enum operation_priorities {
    priority_undefined = 0,
    priority_addition = 1,
    priority_subtraction = 1,
    priority_multiplication = 2,
    priority_division = 2
};


static int is_digit(char ch)
{
    return '0' <= ch && ch <= '9';
}


static int is_operation(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}


static enum operation_priorities priority_of(char operation)
{
    switch (operation) {
        case '+':
            return priority_addition;
        case '-':
            return priority_subtraction;
        case '*':
            return priority_multiplication;
        case '/':
            return priority_division;
    }
    return priority_undefined;
}


static int apply_operation(char operation, int a, int b)
{
    switch (operation) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
    }
    return 0;
}


static void add_number(const char **expression, struct int_stack *values)
{
    int number = 0;
    for (; is_digit(**expression); (*expression)++) {
        number *= 10;
        number += (**expression) - '0';
    }
    int_stack_push(number, values);
}


static void process_remaining_operations(struct int_stack *values,
        struct char_stack *operations)
{
    char op;
    while ((op = char_stack_pop(operations)) != '(') {
        int b = int_stack_pop(values),
            a = int_stack_pop(values);
        int_stack_push(apply_operation(op, a, b), values);
    }
}


static void process_operation(char op, struct int_stack *values,
        struct char_stack *operations)
{
    enum operation_priorities priority = priority_of(op);
    while (priority <= priority_of(operations->top->data)) {
        int b = int_stack_pop(values),
            a = int_stack_pop(values);
        char oper = char_stack_pop(operations);
        int_stack_push(apply_operation(oper, a, b), values);
    }
    char_stack_push(op, operations);
}


int calculate_expression(const char *expression)
{
    int result;
    struct int_stack values;
    struct char_stack operations;

    int_stack_init(&values);

    char_stack_init(&operations);
    char_stack_push('(', &operations);

    while (*expression) {
        char ch = *expression;

        if (is_digit(ch)) {
            add_number(&expression, &values);
            continue;
        }

        if (ch == '(') {
            char_stack_push(ch, &operations);
        } else if (ch == ')') {
            process_remaining_operations(&values, &operations);
        } else if (is_operation(ch)) {
            process_operation(ch, &values, &operations);
        }

        expression++;
    }
    process_remaining_operations(&values, &operations);

    result = int_stack_pop(&values);
    int_stack_destroy(&values);
    char_stack_destroy(&operations);
    return result;
}
