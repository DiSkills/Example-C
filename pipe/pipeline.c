#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


typedef const char *const *cargv_t;
typedef char **argv_t;

typedef enum {
    state_initial,
    state_cmd1,
    state_delimiter,
    state_cmd2,
    state_error
} state_t;

typedef struct {
    const char *name;
    cargv_t argv;
} cmd_t;

typedef struct {
    state_t state;

    cmd_t cmd1,
          cmd2;
} lexer_t;


static const char errmsg[] = "Expected: <cmd> ;; <cmd>\n";


static int is_delimiter(const char *s)
{
    return strcmp(s, ";;") == 0;
}


static void lexer_init(lexer_t *lexer)
{
    lexer->state = state_initial;
}


static void lexer_initial(lexer_t *lexer, cargv_t argv)
{
    if (is_delimiter(*argv)) {
        lexer->state = state_error;
    } else {
        lexer->state = state_cmd1;
        lexer->cmd1.name = argv[0];
        lexer->cmd1.argv = argv;
    }
}


static void lexer_cmd1(lexer_t *lexer, cargv_t argv)
{
    if (is_delimiter(*argv)) {
        lexer->state = state_delimiter;
    }
}


static void lexer_delimiter(lexer_t *lexer, cargv_t argv)
{
    if (is_delimiter(*argv)) {
        lexer->state = state_error;
    } else {
        lexer->state = state_cmd2;
        lexer->cmd2.name = argv[0];
        lexer->cmd2.argv = argv;
    }
}


static void lexer_cmd2(lexer_t *lexer, cargv_t argv)
{
    if (is_delimiter(*argv)) {
        lexer->state = state_error;
    }
}


static void lexer_process(lexer_t *lexer, cargv_t argv)
{
    switch (lexer->state) {
        case state_initial:
            lexer_initial(lexer, argv);
            return;

        case state_cmd1:
            lexer_cmd1(lexer, argv);
            return;

        case state_delimiter:
            lexer_delimiter(lexer, argv);
            return;

        case state_cmd2:
            lexer_cmd2(lexer, argv);
            return;

        case state_error:
            return;
    }
}


static void cmd_exec(const cmd_t *cmd, int fd[2], int stream)
{
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(2);
    }

    if (pid == 0) {
        dup2(fd[stream], stream);
        close(fd[0]);
        close(fd[1]);

        execvp(cmd->name, (argv_t)cmd->argv);
        perror(cmd->name);
        exit(1);
    }
}


static void run(const cmd_t *cmd1, const cmd_t *cmd2)
{
    int fd[2];
    pipe(fd);

    cmd_exec(cmd1, fd, 1);
    cmd_exec(cmd2, fd, 0);

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);
}


int main(int argc, argv_t argv)
{
    lexer_t lexer;
    argv_t delimiter;

    lexer_init(&lexer);
    for (argv++; *argv; argv++) {
        lexer_process(&lexer, (cargv_t)argv);
    }

    if (lexer.state != state_cmd2) {
        fprintf(stderr, errmsg);
        return 1;
    }
    delimiter = (argv_t)lexer.cmd2.argv - 1;
    *delimiter = NULL;

    run(&lexer.cmd1, &lexer.cmd2);
    return 0;
}
