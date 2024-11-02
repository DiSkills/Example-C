#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#define NAME "mydaemon"

enum {
    buffer_size = 4096,
    alarm_interval = 60 * 5
};


static volatile sig_atomic_t sigusr1cnt = 0;
static const char filename[] = "/tmp/" NAME ".log";


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
        syslog(LOG_ERR, "fork: %m");
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
    int fd;
    pid_t pid;
    time_t start;
    char buffer[buffer_size];

    openlog(NAME, 0, LOG_USER);
    demonization();

    fd = open(filename, O_WRONLY|O_CREAT|O_APPEND, 0666);
    if (fd == -1) {
        syslog(LOG_ERR, "%s: %m", filename);
        exit(2);
    }
    pid = getpid();
    start = time(NULL);

    signal(SIGALRM, sigalarm_handler);
    signal(SIGUSR1, sigusr1_handler);

    syslog(LOG_INFO, "Started daemon");
    alarm(alarm_interval);
    for (;;) {
        int sz;
        pause();
        sz = snprintf(buffer, sizeof(buffer),
                      "%s: pid=%d, works %ld seconds, SIGUSR1: %d\n",
                      NAME, pid, time(NULL) - start, sigusr1cnt);
        write(fd, buffer, sz);
        syslog(LOG_INFO, "Added to %s: %s", filename, buffer);
    }
    return 0;
}
