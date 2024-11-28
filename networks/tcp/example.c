#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>


#define SERVER_NAME "mytcp"


static int is_digit(char c)
{
    return '0' <= c && c <= '9';
}


static unsigned short inet_port_aton(const char *s)
{
    unsigned port;

    port = 0;
    for (; *s; s++) {
        if (!is_digit(*s)) {
            return 0;
        }
        port *= 10;
        port += *s - '0';
    }
    if (port > USHRT_MAX) {
        return 0;
    }
    return htons((unsigned short)port);
}


static void reset_leadership()
{
    int pid;

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
    
    closelog();
    return 0;
}
