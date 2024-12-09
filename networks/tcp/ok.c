#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#ifndef INIT_CLIENT_ARRAY_SIZE
#define INIT_CLIENT_ARRAY_SIZE 16
#endif

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 16
#endif

struct server {
    int lsd;

    int *client_array;
    int client_array_size;
};


static int server_init(struct server *serv, int port)
{
    int lsd, i, res, opt;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (lsd == -1) {
        perror("socket");
        return 0;
    }

    opt = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(lsd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        return 0;
    }

    listen(lsd, LISTEN_QLEN);

    serv->lsd = lsd;

    serv->client_array = malloc(sizeof(*serv->client_array) *
                                INIT_CLIENT_ARRAY_SIZE);
    serv->client_array_size = INIT_CLIENT_ARRAY_SIZE;
    for (i = 0; i < serv->client_array_size; i++) {
        serv->client_array[i] = 0;
    }

    return 1;
}

int main(int argc, char **argv)
{
    int ok;
    long port;
    char *endptr;
    struct server serv;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        return 1;
    }

    port = strtol(argv[1], &endptr, 10);
    if (!*argv[1] || *endptr || port < 0 || port > USHRT_MAX) {
        fprintf(stderr, "The port is invalid\n");
        return 2;
    }

    ok = server_init(&serv, port);
    if (!ok) {
        return 3;
    }
    /* TODO run server */
    return 0;
}
