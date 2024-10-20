#include <signal.h>
#include <stdio.h>
#include <unistd.h>


volatile static sig_atomic_t n = 0;
volatile static sig_atomic_t is_caught = 0;


static void handler(int signo)
{
    signal(signo, handler);

    if (signo == SIGINT) {
        n++;
    } else if (signo == SIGUSR1) {
        is_caught = 1;
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
