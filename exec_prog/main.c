#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    pid_t pid;

    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 2;
    }

    if (pid == 0) {
        int fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
        if (fd == -1) {
            perror(argv[1]);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);

        execvp(argv[2], argv + 2);
        perror(argv[2]);
        exit(2);
    }

    wait(NULL);

    return 0;
}
