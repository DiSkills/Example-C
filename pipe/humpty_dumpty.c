#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    int c;
    pid_t pid;
    int fd[2];

    pipe(fd);
    printf("%d %d\n", fd[0], fd[1]);

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);

        printf("Humpty Dumpty sat on a wall,\n");
        printf("Humpty Dumpty had a great fall.\n");
        printf("All the king’s horses and all the king’s men\n");
        printf("Couldn’t put Humpty together again.\n");

        exit(0);
    }
    close(fd[1]);
    dup2(fd[0], 0);

    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    wait(NULL);
    return 0;
}
