#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


enum {
    alarm_interval = 60 * 5
};


static volatile sig_atomic_t sigusr1cnt = 0;
static const char filename[] = "/tmp/mydaemon.log";


static void sigalarm_handler(int signo)
{
    signal(SIGALRM, sigalarm_handler);

    alarm(alarm_interval);
}


static void sigusr1_handler(int signo)
{
    signal(SIGUSR1, sigusr1_handler);

    sigusr1cnt++;

    alarm(alarm_interval);
}


static void reset_leader()
{
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid > 0) {
        exit(0);
    }
}


static void demonization()
{
    close(0);
    close(1);
    close(2);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);

    chdir("/");

    reset_leader();
    setsid();

    reset_leader();
}


int main()
{
    FILE *f;
    pid_t pid;
    time_t start;

    demonization();

    signal(SIGALRM, sigalarm_handler);
    signal(SIGUSR1, sigusr1_handler);

    alarm(alarm_interval);

    f = fopen(filename, "a");
    pid = getpid();
    start = time(NULL);
    for (;;) {
        pause();
        fprintf(f, "mydaemon: %d, works %ld seconds, SIGUSR1: %d\n",
                pid, time(NULL) - start, sigusr1cnt);
        fflush(f);
    }
    return 0;
}
