#include <stdio.h>

const char *substr(const char *string, const char *substring)
{
    for (; *string; string++) {
        const char *str = string,
              *sub = substring;

        for (; *sub; sub++, str++) {
            if (!*str)
                return NULL;
            if (*str != *sub)
                break;
        }

        if (!*sub)
            return string;
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Too few arguments.\n");
        return 1;
    }
    char *sub = argv[1];
    for (argv += 2; *argv; argv++) {
        if (substr(*argv, sub)) {
            printf("%s\n", *argv);
        }
    }
    return 0;
}
