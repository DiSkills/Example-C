#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "inet_port_aton.h"


typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;

enum {
    BUFFER_SIZE = 1500
};

typedef struct {
    unsigned long long requests,
                       total;
} info_t;


static void dgram(int sd)
{
    int res;
    char buf[BUFFER_SIZE];
    sockaddr_in_t client;
    socklen_t client_size;
    static info_t info = { 0LL, 0LL };

    client_size = sizeof(client);
    res = recvfrom(sd, buf, sizeof(buf), 0,
                   (sockaddr_t *)&client, &client_size);
    if (res == -1) {
        perror("recvfrom");
        return;
    }
    info.requests++;
    info.total += res;

    sprintf(buf, "%llu %llu", info.requests, info.total);
    res = sendto(sd, buf, strlen(buf), 0,
                 (sockaddr_t *)&client, client_size);
    if (res == -1) {
        perror("sendto");
        return;
    }
}


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
