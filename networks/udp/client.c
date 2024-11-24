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
    TIMEOUT_SECONDS = 15
};


static void send_datagram(unsigned ip, unsigned short port, int size)
{
    int sd;
    int res;
    char buf[BUFFER_SIZE];
    struct sockaddr_in addr;
    struct timeval tv;
    socklen_t lenaddr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = port;

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    res = sendto(sd, buf, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    tv.tv_sec = TIMEOUT_SECONDS;
    setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    lenaddr = sizeof(addr);
    res = recvfrom(sd, buf, sizeof(buf), 0,
                   (struct sockaddr *)&addr, &lenaddr);
    if (res == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }
    printf("%.*s\n", res, buf);

    close(sd);
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
