#include <stdio.h>
#include <stdlib.h>

struct fibonacci_machine {
    int f1, f2;
};

static struct fibonacci_machine *fibonacci_machine_init()
{
    struct fibonacci_machine *machine = malloc(sizeof(*machine));
    machine->f1 = 1;
    machine->f2 = 1;
    return machine;
}

static int fibonacci_machine_step(struct fibonacci_machine *machine)
{
    int res = machine->f1;
    machine->f1 = machine->f2;
    machine->f2 += res;
    return res;
}

int main(int argc, char **argv)
{
    int n;
    struct fibonacci_machine *machine;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <n>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    machine = fibonacci_machine_init();
    for (; n > 0; n--) {
        printf("%d\n", fibonacci_machine_step(machine));
    }
    free(machine);
    return 0;
}
