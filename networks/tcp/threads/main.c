#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char **argv)
{
    int lsd;
    long port;
    char *endptr;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        return 1;
    }

    port = strtol(argv[1], &endptr, 10);
    if (!*argv[1] || *endptr) {
        fprintf(stderr, "The port is invalid\n");
        return 2;
    }

    lsd = server_init(port);
    if (lsd == -1) {
        return 3;
    }
    return 0;
}
