#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <limits.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}


static int atop(const char *s, unsigned short *port)
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
    *port = htons(number);
    return 1;
}


static int is_printable(char c)
{
    return (32 <= c && c <= 126) || c == '\n' || c == '\t';
}


static void handle_datagram(int sd)
{
    int i;
    int rr;
    char buf[512];
    struct sockaddr_in from;
    socklen_t fromlen;

    fromlen = sizeof(from);
    rr = recvfrom(sd, buf, sizeof(buf), 0,
                  (struct sockaddr *)&from, &fromlen);
    if (rr == -1) {
        perror("recvfrom");
        return;
    }

    printf("%s:%hu ", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
    for (i = 0; i < rr; i++) {
        putchar(is_printable(buf[i]) ? buf[i] : '?');
    }
    putchar('\n');
}


static void sigint_handler(int signo)
{
    exit(0);
}


int main(int argc, char **argv)
{
    int sd;
    int ok, br;
    struct sockaddr_in addr;

    signal(SIGINT, sigint_handler);

    if (argc != 2) {
        fprintf(stderr, "Expected port\n");
        exit(1);
    }

    ok = atop(argv[1], &addr.sin_port);
    if (!ok) {
        fprintf(stderr, "The port is not an unsigned short\n");
        exit(2);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1) {
        perror("socket");
        exit(3);
    }

    br = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
    if (br == -1) {
        perror("bind");
        exit(4);
    }

    for (;;) {
        handle_datagram(sd);
    }

    close(sd);
    return 0;
}
