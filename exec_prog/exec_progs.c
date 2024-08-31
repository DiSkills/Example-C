#include <stdio.h>
#include <string.h>
#include <unistd.h>


static int is_separator(const char *s)
{
    return strcmp(s, ";;") == 0;
}


static void print_cmd(char * const * argv)
{
    for (; *argv; argv++) {
        printf("%s ", *argv);
    }
    putchar('\n');
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
            print_cmd(argv);
        }
        argv = p + 1;
    }

    return 0;
}
