#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


static volatile sig_atomic_t pipe_is_closed = 0;

static void sigpipe_handler(int signo)
{
    pipe_is_closed = 1;
}

static void run_child(char **argv, int *fd)
{
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(2);
    }
    if (pid == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);

        execvp(argv[0], argv);
        perror(argv[0]);
        exit(1);
    }
}

int main(int argc, char **argv)
{
    int i, status;
    FILE *fchld;
    int fd[2];

    signal(SIGPIPE, sigpipe_handler);
    if (argc < 2) {
        fprintf(stderr, "Expected: <cmd>\n");
        return 1;
    }

    pipe(fd);
    run_child(argv + 1, fd);
    close(fd[0]);

    fchld = fdopen(fd[1], "w");
    for (i = 0; !pipe_is_closed && i < 1000000; i++) {
        fprintf(fchld, "%d\n", i + 1);
    }
    fclose(fchld);

    wait(&status);
    if (WIFEXITED(status)) {
        printf("Exited: %d\n", WEXITSTATUS(status));
    } else {
        printf("Terminated: %d\n", WTERMSIG(status));
    }
    return 0;
}
