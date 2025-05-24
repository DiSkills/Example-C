#include <stdlib.h>
#include <stdio.h>

struct bincoef {
    int n, k, value;
};

struct state_machine {
    int k, n;
    int *next /* n + 2 */, *cur /* n + 1 */;
};

static struct state_machine *state_machine_init()
{
    struct state_machine *machine = malloc(sizeof(*machine));
    machine->k = 0;
    machine->n = 0;

    machine->cur = malloc(sizeof(int) * 1);
    *machine->cur = 1;

    machine->next = malloc(sizeof(int) * 2);
    machine->next[0] = 1;
    machine->next[1] = 1;
    return machine;
}

static void state_machine_step(struct state_machine *machine,
        struct bincoef *res)
{
    int k;
    res->k = machine->k;
    res->n = machine->n;
    res->value = machine->cur[machine->k];

    if (machine->k != machine->n) {
        machine->k++;
        return;
    }

    free(machine->cur);
    machine->cur = machine->next;
    machine->k = 0;
    machine->n++;

    machine->next = malloc(sizeof(int) * (machine->n + 2));
    machine->next[0] = 1;
    for (k = 1; k <= machine->n; k++) {
        machine->next[k] = machine->cur[k - 1] + machine->cur[k];
    }
    machine->next[k] = 1;
}

static void state_machine_del(struct state_machine *machine)
{
    free(machine->cur);
    free(machine->next);
    free(machine);
}

int main(int argc, char **argv)
{
    int n, i, j;
    struct state_machine *machine;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <number-of-rows>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    machine = state_machine_init();
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            struct bincoef res;
            state_machine_step(machine, &res);
            printf("(%d, %d, %d)\n", res.n, res.k, res.value);
        }
    }
    state_machine_del(machine);
    return 0;
}
