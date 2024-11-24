#include <unistd.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "inet_port_aton.h"


enum {
    BUFFER_SIZE = 1500,
};


static void send_datagram(unsigned ip, unsigned short port, int size)
{
}


int main(int argc, char **argv)
{
    unsigned ip;
    unsigned short port;
    int size;

    if (argc != 4) {
        fprintf(stderr, "Expected: %s <ip> <port> <length>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ip = inet_addr(argv[1]);
    if (ip == -1) {
        fprintf(stderr, "An invalid IP address was entered\n");
        exit(EXIT_FAILURE);
    }

    port = inet_port_aton(argv[2]);
    if (!port) {
        fprintf(stderr, "An incorrect port was entered\n");
        exit(EXIT_FAILURE);
    }

    size = atoi(argv[3]);
    if (size <= 0 || size > BUFFER_SIZE) {
        fprintf(stderr, "An invalid datagram size was entered\n");
        exit(EXIT_FAILURE);
    }

    send_datagram(ip, port, size);
    return 0;
}
