#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(A, B)                                                           \
    do {                                                                     \
        int tmp = (A);                                                       \
        (A) = (B);                                                           \
        (B) = tmp;                                                           \
    } while (0)

static void print_numbers(int *arr, int length)
{
    int i;
    for (i = 0; i < length; i++) {
        printf("%d%c", arr[i], i == length - 1 ? '\n' : ' ');
    }
}

enum node_states { start, completed };

struct node {
    int i;
    enum node_states state;
};

struct permutations_state_machine {
    struct node *nodes;
    int top, n;
};

static struct permutations_state_machine *permutations_init(int n)
{
    int i;
    struct permutations_state_machine *machine;

    machine = malloc(sizeof(*machine));
    machine->nodes = malloc(n * sizeof(*machine->nodes));
    for (i = 0; i < n; i++) {
        machine->nodes[i].i = i;
        machine->nodes[i].state = start;
    }
    machine->n = n;
    machine->top = 0;
    return machine;
}

static int permutations__step(struct permutations_state_machine *machine,
        int *arr)
{
    struct node *top;

    if (machine->top == -1) {
        memset(arr, 0, machine->n * sizeof(*arr));
        return 1;
    }

    top = machine->nodes + machine->top;
    switch (top->state) {
    case start:
        SWAP(arr[machine->top], arr[top->i]);
        top->state = completed;
        if (machine->top < machine->n - 1) {
            machine->top++;
            machine->nodes[machine->top].i = machine->top;
            machine->nodes[machine->top].state = start;
        } else {
            return 1;
        }
        break;
    case completed:
        SWAP(arr[machine->top], arr[top->i]);
        if (top->i >= machine->n - 1) {
            machine->top--;
        } else {
            top->i++;
            top->state = start;
        }
        break;
    }
    return 0;
}

static void permutations_step(struct permutations_state_machine *machine,
        int *arr)
{
    int res;
    do {
        res = permutations__step(machine, arr);
    } while (!res);
}

static void permutations_del(struct permutations_state_machine *machine)
{
    free(machine->nodes);
    free(machine);
}

int main()
{
    int i, arr[] = { 1, 2, 3, 4 };
    int len = sizeof(arr) / sizeof(*arr);
    struct permutations_state_machine *machine = permutations_init(len);
    for (i = 0; i < 25; i++) {
        permutations_step(machine, arr);
        print_numbers(arr, len);
    }
    permutations_del(machine);
    return 0;
}
