#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


enum {
    interval = 5
};


static const char msg[] = "Are you sleeping?\n";


static volatile sig_atomic_t lines = 0,
                             symbols = 0,
                             exit_is_active = 0;


static void sigalrm_handler(int signum)
{
    int save_errno = errno;

    signal(SIGALRM, sigalrm_handler);

    exit_is_active = 0;
    write(1, msg, sizeof(msg) - 1);

    alarm(interval);
    errno = save_errno;
}


static int int_to_str(int number, char *s, int size)
{
    int i;
    char buffer[10];

    i = 0;
    do {
        buffer[i] = number % 10 + '0';
        number /= 10;
        i++;
    } while (i < sizeof(buffer) && number);

    if (i >= size) {
        return -1;
    }

    size = i;
    for (i = 0; i < size; i++) {
        s[i] = buffer[size - 1 - i];
    }
    return size;
}


static void print(int lines, int symbols)
{
    int len1,
        len2;
    char buffer[22];

    len1 = int_to_str(lines, buffer, sizeof(buffer));
    if (len1 == -1) {
        return;
    }
    buffer[len1] = ' ';
    len1++;

    len2 = int_to_str(symbols, buffer + len1, sizeof(buffer) - len1);
    if (len2 == -1) {
        return;
    }
    buffer[len1 + len2] = '\n';

    write(1, buffer, len1 + len2 + 1);
}


static void sigint_handler(int signum)
{
    int save_errno = errno;

    signal(SIGINT, sigint_handler);

    if (exit_is_active) {
        exit(0);
    }
    exit_is_active = 1;

    print(lines, symbols);

    alarm(interval);
    errno = save_errno;
}


static int count_new_lines(const char *buffer, int n)
{
    int cnt = 0;

    for (; n; n--, buffer++) {
        if (*buffer == '\n') {
            cnt++;
        }
    }
    return cnt;
}


int main()
{
    char buffer[4096];

    signal(SIGALRM, sigalrm_handler);
    signal(SIGINT, sigint_handler);

    alarm(interval);
    for (;;) {
        int rr = read(0, buffer, sizeof(buffer));

        if (rr != -1) {
            alarm(0);
            exit_is_active = 0;
            lines += count_new_lines(buffer, rr);
            symbols += rr;
        }
    }

    return 0;
}
