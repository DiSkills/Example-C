#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


static volatile sig_atomic_t exit_is_active = 0;
static volatile sig_atomic_t symbol = '+';


static void print_char()
{
    write(1, (char *)&symbol, 1);
}


static void sigalarm_handler(int signum)
{
    signal(SIGALRM, sigalarm_handler);

    exit_is_active = 0;
    print_char();
    alarm(1);
}


static void sigint_handler(int signum)
{
    signal(SIGINT, sigint_handler);

    if (exit_is_active) {
        exit(0);
    }

    symbol = '-';
    exit_is_active = 1;
    print_char();
    alarm(1);
}


static void sigquit_handler(int signum)
{
    signal(SIGQUIT, sigquit_handler);

    symbol = '+';
    print_char();
    alarm(1);
}


int main()
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    signal(SIGALRM, sigalarm_handler);

    alarm(1);
    for (;;) {
        pause();
    }

    return 0;
}
