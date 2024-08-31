#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


static int is_separator(const char *s)
{
    return strcmp(s, ";;") == 0;
}


static void execute_command(const char *cmd, char * const * argv)
{
    int pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        execvp(cmd, argv);
        perror(cmd);
        exit(1);
    }
}


void print_successful_commands()
{
    int pid,
        status;

    while ((pid = wait(&status)) != -1) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("%d\n", pid);
        }
    }
}


int main(int argc, char **argv)
{
    char **p;

    for (argv++, p = argv; argc; argc--, p++) {
        if (*p && !is_separator(*p)) {
            continue;
        }

        *p = NULL;
        if (argv != p) {
            execute_command(argv[0], argv);
        }
        argv = p + 1;
    }
    print_successful_commands();

    return 0;
}
