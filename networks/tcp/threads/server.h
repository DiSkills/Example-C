#ifndef SERVER_H
#define SERVER_H

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 16
#endif

int server_init(int port);

#endif
