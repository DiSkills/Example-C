#include <stdio.h>

void int_file_sum(FILE *f, int *sum, int *count)
{
    int n;
    *sum = 0;
    *count = 0;

    while (fscanf(f, "%d", &n) == 1) {
        *sum += n;
        (*count)++;
    }
}

int main(int argc, char **argv)
{
    FILE *f;
    int sum, count;

    if (argc < 2) {
        fprintf(stderr, "No parameter given\n");
        return 1;
    }

    f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 2;
    }

    int_file_sum(f, &sum, &count);
    printf("Sum=%d, count=%d\n", sum, count);
    return 0;
}
