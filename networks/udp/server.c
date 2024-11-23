#include <stdio.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "inet_port_aton.h"


typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;


int main(int argc, char **argv)
{
    int sd;
    int ok;
    sockaddr_in_t addr;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    addr.sin_port = inet_port_aton(argv[1]);
    if (!addr.sin_port) {
        fprintf(stderr, "An incorrect port was entered\n");
        return 2;
    }

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1) {
        perror("socket");
        return 3;
    }
    ok = bind(sd, (sockaddr_t *)&addr, sizeof(addr));
    if (ok == -1) {
        perror("bind");
        return 4;
    }

    close(sd);
    return 0;
}
