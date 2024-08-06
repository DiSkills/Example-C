#include "calls.h"

enum {
    buffer_size = 4096
};

static char buffer[buffer_size];
static const char helpmsg[] = "Usage: copy <src> <dest>\n";
static const char err1msg[] = "Couldn't open source file for reading\n";
static const char err2msg[] = "Couldn't open destination file for writing\n";


int main(int argc, char **argv)
{
    int s,
        fdsrc,
        fddest;

    if (argc != 3) {
        write(2, helpmsg, sizeof(helpmsg) - 1);
        return 1;
    }

    fdsrc = open(argv[1], O_RDONLY, 0666);
    if (fdsrc == -1) {
        write(2, err1msg, sizeof(err1msg) - 1);
        return 2;
    }

    fddest = open(argv[2], O_TRUNC + O_CREAT + O_WRONLY, 0666);
    if (fddest == -1) {
        write(2, err2msg, sizeof(err2msg) - 1);
        return 3;
    }

    while ((s = read(fdsrc, buffer, buffer_size)) > 0) {
        write(fddest, buffer, s);
    }

    close(fdsrc);
    close(fddest);
    return 0;
}
