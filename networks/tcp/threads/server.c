#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

#include "server.h"

int server_init(int port)
{
    int lsd, opt, res;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, 0);
    if (lsd == -1) {
        perror("socket");
        return -1;
    }

    opt = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(lsd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        return -1;
    }

    listen(lsd, LISTEN_QLEN);
    return lsd;
}
