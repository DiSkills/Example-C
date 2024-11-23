#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

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
        syslog(LOG_ERR, "recvfrom: %m");
        return;
    }
    syslog(LOG_INFO, "A datagram was received from %s:%hu",
           inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    info.requests++;
    info.total += res;

    sprintf(buf, "%llu %llu", info.requests, info.total);
    res = sendto(sd, buf, strlen(buf), 0,
                 (sockaddr_t *)&client, client_size);
    if (res == -1) {
        syslog(LOG_ERR, "sendto: %m");
        return;
    }
    syslog(LOG_INFO, "\"%s\" was sent to %s:%hu", buf,
           inet_ntoa(client.sin_addr), ntohs(client.sin_port));
}


static void run_server(unsigned short port)
{
    int sd;
    int res;
    sockaddr_in_t addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = port;

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1) {
        syslog(LOG_ERR, "socket: %m");
        exit(EXIT_FAILURE);
    }
    res = bind(sd, (sockaddr_t *)&addr, sizeof(addr));
    if (res == -1) {
        syslog(LOG_ERR, "bind: %m");
        exit(EXIT_FAILURE);
    }
    syslog(LOG_INFO, "The UDP server was started on port %hu", ntohs(port));

    for (;;) {
        dgram(sd);
    }

    close(sd);
}


static void reset_leader()
{
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        syslog(LOG_ERR, "fork: %m");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
}


static void daemonize()
{
    close(0);
    close(1);
    close(2);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);

    chdir("/");

    reset_leader();
    setsid();
    reset_leader();
}


int main(int argc, char **argv)
{
    unsigned short port;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    port = inet_port_aton(argv[1]);
    if (!port) {
        fprintf(stderr, "An incorrect port was entered\n");
        exit(EXIT_FAILURE);
    }

    openlog(argv[0], 0, LOG_USER);
    daemonize();

    run_server(port);

    closelog();
    return 0;
}
