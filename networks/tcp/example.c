#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>


static int is_digit(char c)
{
    return '0' <= c && c <= '9';
}


static unsigned short inet_port_aton(const char *s)
{
    unsigned port;

    port = 0;
    for (; *s; s++) {
        if (!is_digit(*s)) {
            return 0;
        }
        port *= 10;
        port += *s - '0';
    }
    if (port > USHRT_MAX) {
        return 0;
    }
    return htons((unsigned short)port);
}


int main(int argc, char **argv)
{
    unsigned short port;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = inet_port_aton(argv[1]);
    if (!port) {
        fprintf(stderr, "An incorrect port was entered\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
