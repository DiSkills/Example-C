#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


enum {
    buffer_size = 4096
};


static void encrypt_buffer(void *buf, int len, unsigned key)
{
    unsigned *data = buf;
    int i,
        size = len / sizeof(*data);
    for (i = 0; i < size; i++) {
        data[i] ^= key;
    }
    if (len % sizeof(*data)) {
        data[i] ^= key;
    }
}


static void encrypt(int fd, unsigned key)
{
    int rc;
    char buf[buffer_size];

    while ((rc = read(fd, buf, sizeof(buf))) > 0) {
        lseek(fd, -rc, SEEK_CUR);
        encrypt_buffer(buf, rc, key);
        write(fd, buf, rc);
    }
}


int main(int argc, char **argv)
{
    int fd,
        res;
    char *filename;

    if (argc != 3) {
        fputs("Expected: <filename> <key>\n", stderr);
        return 1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror(filename);
        return 2;
    }

    encrypt(fd, atoi(argv[2]));

    res = close(fd);
    if (res == -1) {
        perror(filename);
        return 3;
    }
    return 0;
}
