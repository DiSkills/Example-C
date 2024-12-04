#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/time.h>


enum {
    BUFFER_SIZE = 4096,
    TIMEOUT_SECONDS = 5,
    TIMEOUT_USECONDS = 0
};


static const char msg[] = "Are you sleeping?\n";


static volatile sig_atomic_t lines = 0,
                             symbols = 0,
                             is_exit_active = 0;


static void print(int lines, int symbols)
{
    int sz;
    char buf[32];

    sz = snprintf(buf, sizeof(buf), "%d %d\n", lines, symbols);
    write(1, buf, sz);
}


static void sigint_handler(int signo)
{
    int save_errno = errno;

    signal(SIGINT, sigint_handler);

    if (is_exit_active) {
        exit(EXIT_SUCCESS);
    }
    is_exit_active = 1;
    print(lines, symbols);

    errno = save_errno;
}


static int count_lines(const char *buf, int n)
{
    int cnt = 0;

    for (; n; n--, buf++) {
        if (*buf == '\n') {
            cnt++;
        }
    }
    return cnt;
}


static void handle_timeout()
{
    write(1, msg, sizeof(msg) - 1);
}


static void handle_input()
{
    int rc;
    char buf[BUFFER_SIZE];

    rc = read(0, buf, sizeof(buf));
    if (rc > 0) {
        lines += count_lines(buf, rc);
        symbols += rc;
    }
}


int main()
{
    signal(SIGINT, sigint_handler);

    for (;;) {
        int res;
        fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        timeout.tv_sec = TIMEOUT_SECONDS;
        timeout.tv_usec = TIMEOUT_USECONDS;

        res = select(0 + 1, &readfds, NULL, NULL, &timeout);
        if (res == -1) {
            if (errno != EINTR) {
                perror("select");
                exit(EXIT_FAILURE);
            }
            continue;
        }

        is_exit_active = 0;

        if (res == 0) {
            handle_timeout();
            continue;
        }

        handle_input();
    }
    return 0;
}
