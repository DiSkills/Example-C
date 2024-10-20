#include <signal.h>
#include <stdio.h>
#include <unistd.h>


volatile static sig_atomic_t is_caught = 0;


static void handler(int signo)
{
    is_caught = 1;
}


int main()
{
    signal(SIGINT, handler);

    printf("Press Ctrl-C to quit\n");
    while (!is_caught) {
        pause();
    }
    printf("Good bye\n");

    return 0;
}
