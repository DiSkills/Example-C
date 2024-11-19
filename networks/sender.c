#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}


static int atop(const char *s, in_port_t *port)
{
    unsigned number;

    number = 0;
    for (; *s; s++) {
        if (!is_digit(*s)) {
            return 0;
        }
        number *= 10;
        number += *s - '0';
    }
    if (number > USHRT_MAX) {
        return 0;
    }
    *port = number;
    return 1;
}


int main(int argc, char **argv)
{
    int ok;
    in_port_t port;
    struct sockaddr_in addr;

    if (argc != 4) {
        fprintf(stderr, "Expected IP address, port, data\n");
        exit(1);
    }
    addr.sin_family = AF_INET;

    ok = inet_aton(argv[1], &addr.sin_addr);
    if (!ok) {
        fprintf(stderr, "IP address is incorrect\n");
        exit(2);
    }

    ok = atop(argv[2], &port);
    if (!ok) {
        fprintf(stderr, "The port is not an unsigned short\n");
        exit(3);
    }
    addr.sin_port = htons(port);

    return 0;
}
