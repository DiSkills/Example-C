#include <errno.h>
#include <signal.h>
#include <unistd.h>


volatile static sig_atomic_t n = 0;

static const char msg[] = "Press it again, I like it\n";


static void handler(int s)
{
    int save_errno = errno;

    signal(SIGINT, handler);

    write(1, msg, sizeof(msg) - 1);
    n++;

    errno = save_errno;
}


int main()
{
    signal(SIGINT, handler);

    while (n < 25) {
        pause();
    }

    return 0;
}
