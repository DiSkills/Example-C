#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    long port;
    char *endptr;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        return 1;
    }

    port = strtol(argv[1], &endptr, 10);
    if (!*argv[1] || *endptr || port < 0 || port > USHRT_MAX) {
        fprintf(stderr, "The port is invalid\n");
        return 2;
    }
    return 0;
}
