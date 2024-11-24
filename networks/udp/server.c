#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "inet_port_aton.h"


#define SERVER_NAME "udp-server"

enum {
    BUFFER_SIZE = 1500
};

typedef struct {
    unsigned request_count;
    unsigned long long total_data;
} server_stats_t;


static void handle_datagram(int sd, server_stats_t *stats)
{
    int res;
    int sz;
    char buf[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_size;

    client_size = sizeof(client_addr);
    res = recvfrom(sd, buf, sizeof(buf), 0,
                   (struct sockaddr *)&client_addr, &client_size);
    if (res == -1) {
        syslog(LOG_WARNING, "recvfrom: %m");
        return;
    }
    syslog(LOG_INFO, "A datagram was received from %s:%hu",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    stats->request_count++;
    stats->total_data += res;
    sz = sprintf(buf, "%u %llu", stats->request_count, stats->total_data);

    res = sendto(sd, buf, sz, 0,
                 (struct sockaddr *)&client_addr, client_size);
    if (res == -1) {
        syslog(LOG_WARNING, "sendto: %m");
        return;
    }
    syslog(LOG_INFO, "\"%s\" was sent to %s:%hu", buf,
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}


static void run_server(unsigned short port)
{
    int sd;
    int res;
    struct sockaddr_in server_addr;
    server_stats_t stats;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = port;

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1) {
        syslog(LOG_ERR, "socket: %m");
        exit(EXIT_FAILURE);
    }
    res = bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res == -1) {
        syslog(LOG_ERR, "bind: %m");
        exit(EXIT_FAILURE);
    }
    syslog(LOG_INFO, "The UDP server was started on port %hu", ntohs(port));

    stats.request_count = 0;
    stats.total_data = 0;
    for (;;) {
        handle_datagram(sd, &stats);
    }

    close(sd);
}


static void reset_leadership()
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

    reset_leadership();
    setsid();
    reset_leadership();
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

    openlog(SERVER_NAME, 0, LOG_USER);

    daemonize();
    run_server(port);

    closelog();
    return 0;
}
