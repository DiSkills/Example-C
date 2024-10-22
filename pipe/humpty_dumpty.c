#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


static const char msg1[] = "Humpty Dumpty sat on a wall,\n",
                  msg2[] = "Humpty Dumpty had a great fall.\n",
                  msg3[] = "All the king’s horses and all the king’s men\n",
                  msg4[] = "Couldn’t put Humpty together again.\n";


int main()
{
    int rc;
    char buffer[4096];
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

        write(fd[1], msg1, sizeof(msg1) - 1);
        write(fd[1], msg2, sizeof(msg2) - 1);
        write(fd[1], msg3, sizeof(msg3) - 1);
        write(fd[1], msg4, sizeof(msg4) - 1);

        exit(0);
    }
    close(fd[1]);

    while ((rc = read(fd[0], buffer, sizeof(buffer))) > 0) {
        write(1, buffer, rc);
    }
    wait(NULL);
    return 0;
}
