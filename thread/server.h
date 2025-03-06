#ifndef SERVER_H
#define SERVER_H

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 16
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 512
#endif

int server_init(int port);
int server_run(int lsd);

#endif
