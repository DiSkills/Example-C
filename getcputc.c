#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *from, *to;
    int line_count, ch;

    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    from = fopen(argv[1], "r");
    if (!from) {
        perror(argv[1]);
        return 2;
    }

    to = fopen(argv[2], "w");
    if (!to) {
        perror(argv[2]);
        return 3;
    }

    line_count = 0;
    while ((ch = fgetc(from)) != EOF) {
        fputc(ch, to);
        if (ch == '\n')
            line_count++;
        if (line_count >= 10)
            break;
    }

    fclose(from);
    fclose(to);
    return 0;
}
