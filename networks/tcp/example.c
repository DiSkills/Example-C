#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>


#define SERVER_NAME "mytcp"

enum {
    listen_queue_len = 16,
    buffer_size = 256
};


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


static void handle_request(int clsd, const struct sockaddr_in *addr)
{
    int wc, res;
    char buf[buffer_size];
    time_t now;

    now = time(NULL);
    wc = snprintf(buf, sizeof(buf), "Time: %sIP: %s:%hu\n", ctime(&now),
                  inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    res = write(clsd, buf, wc);
    if (res == -1) {
        syslog(LOG_WARNING, "write: %m");
        return;
    }
    syslog(LOG_INFO, "The message was sent: %s", buf);
}


static void run_server(unsigned short port)
{
    int lsd;
    int res;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (lsd == -1) {
        syslog(LOG_ERR, "socket: %m");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = port;

#ifdef DEBUG
    int opt = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    res = bind(lsd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        syslog(LOG_ERR, "bind: %m");
        exit(EXIT_FAILURE);
    }

    listen(lsd, listen_queue_len);
    syslog(LOG_INFO, "Server was started on port %hu", ntohs(port));

    for (;;) {
        int clsd;
        socklen_t addrlen;

        addrlen = sizeof(addr);
        clsd = accept(lsd, (struct sockaddr *)&addr, &addrlen);
        handle_request(clsd, &addr);
        close(clsd);
    }

    close(lsd);
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
