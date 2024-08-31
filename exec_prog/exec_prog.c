#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    int wr,
        pid,
        status;

    if (argc < 2) {
        fputs("Command not found\n", stderr);
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 2;
    }

    if (pid == 0) {
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        return 3;
    }

    wr = wait(&status);
    if (wr == -1) {
        perror("wait");
        return 4;
    }

    if (WIFEXITED(status)) {
        printf("Process exited with code=%d\n", WEXITSTATUS(status));
    } else {
        printf("Process killed with code=%d\n", WTERMSIG(status));
    }
    return 0;
}
