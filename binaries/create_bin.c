#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    int fd;
    int number;

    if (argc != 2) {
        fputs("Filename expected\n", stderr);
        return 1;
    }

    fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fd == -1) {
        perror(argv[1]);
        return 2;
    }

    while (scanf("%d", &number) == 1) {
        write(fd, &number, sizeof(number));
    }

    close(fd);
    return 0;
}
