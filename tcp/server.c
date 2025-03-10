#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "server.h"

static void server_accept_client(struct server *serv)
{
    int fd, i;

    fd = accept(serv->lsd, NULL, NULL);
    if (fd == -1) {
        perror("accept");
        return;
    }

    if (fd >= serv->session_array_size) {
        int newlen = serv->session_array_size;
        while (fd >= newlen) {
            newlen += INIT_SESSION_ARRAY_SIZE;
        }
        serv->session_array = realloc(serv->session_array,
                newlen * sizeof(*serv->session_array));
        for (i = serv->session_array_size; i < newlen; i++) {
            serv->session_array[i] = NULL;
        }
        serv->session_array_size = newlen;
    }
    serv->session_array[fd] = session_init(fd, &serv->value);
}

static void server_close_client(struct server *serv, int fd)
{
    struct session *sess = serv->session_array[fd];

    close(fd);
    session_del(sess);
    free(sess);
    serv->session_array[fd] = NULL;
}

int server_init(struct server *serv, int port)
{
    int i, lsd, opt, res;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, 0);
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
    serv->value = 0;

    serv->session_array =
        malloc(INIT_SESSION_ARRAY_SIZE * sizeof(*serv->session_array));
    serv->session_array_size = INIT_SESSION_ARRAY_SIZE;
    for (i = 0; i < serv->session_array_size; i++) {
        serv->session_array[i] = NULL;
    }
    return 1;
}

int server_run(struct server *serv)
{
    for (;;) {
        int fd, res;
        fd_set readfds;
        int maxfd = serv->lsd;

        FD_ZERO(&readfds);
        FD_SET(serv->lsd, &readfds);
        for (fd = 0; fd < serv->session_array_size; fd++) {
            if (serv->session_array[fd]) {
                FD_SET(fd, &readfds);
                if (fd > maxfd) {
                    maxfd = fd;
                }
            }
        }

        res = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (res == -1) {
            perror("select");
            return 4;
        }

        if (FD_ISSET(serv->lsd, &readfds)) {
            server_accept_client(serv);
        }

        for (fd = 0; fd < serv->session_array_size; fd++) {
            if (serv->session_array[fd] && FD_ISSET(fd, &readfds)) {
                int srr = session_receive(serv->session_array[fd]);
                if (!srr) {
                    server_close_client(serv, fd);
                }
            }
        }
    }
    return 0;
}
