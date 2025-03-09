#include <stdio.h>

#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t n = 0;
static volatile sig_atomic_t is_caught = 0;

static void handler(int signo)
{
    signal(signo, handler);

    switch (signo) {
    case SIGINT:
        n++;
        break;
    case SIGUSR1:
        is_caught = 1;
        break;
    }
}


int main()
{
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);

    for (;;) {
        pause();

        if (is_caught) {
            printf("%d\n", n);
            is_caught = 0;
        }
    }
    return 0;
}
