#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


typedef void (*pcallback)(char, void*);


void write_callback(char c, void *userdata)
{
    fputc(c, (FILE*)userdata);
}


void ignore_callback(char c, void *userdata)
{
}


int process_line_from_stream(FILE *file, pcallback callback, void *userdata)
{
    int ch,
        count = 1;
    while ((ch = fgetc(file)) != '\n') {
        callback(ch, userdata);
        count++;
    }
    callback('\n', userdata);
    return count;
}


int main(int argc, char **argv)
{
    int c,
        fdlens,
        res;
    FILE *src,
         *dest;

    if (argc < 4) {
        fputs("Expected: <src> <dest> <bin>\n", stderr);
        return 1;
    }

    src = fopen(argv[1], "r");
    if (!src) {
        perror(argv[1]);
        return 2;
    }
    dest = fopen(argv[2], "w");
    if (!dest) {
        perror(argv[2]);
        return 3;
    }
    fdlens = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fdlens == -1) {
        perror(argv[3]);
        return 4;
    }

    while ((c = fgetc(src)) != EOF) {
        ungetc(c, src);
        if (c == ' ') {
            int len = process_line_from_stream(src, write_callback, dest);
            write(fdlens, &len, sizeof(int));
        } else {
            process_line_from_stream(src, ignore_callback, NULL);
        }
    }

    res = close(fdlens);
    if (res == -1) {
        perror(argv[3]);
        return 5;
    }
    res = fclose(dest);
    if (res == -1) {
        perror(argv[2]);
        return 6;
    }
    fclose(src);
    return 0;
}
