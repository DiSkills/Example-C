#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/select.h>


enum {
    BUFFER_SIZE = 1024
};


static int transfer_partial_data(int fdin, int fdout)
{
    int rc;
    char buf[BUFFER_SIZE];

    rc = read(fdin, buf, sizeof(buf));
    if (rc > 0) {
        write(fdout, buf, rc);
    }
    return rc;
}


static void start_chat(const char *f1, const char *f2)
{
    int flags;
    int fdin, fdout;

    fdin = open(f1, O_RDONLY | O_NONBLOCK);
    if (fdin == -1) {
        perror(f1);
        exit(EXIT_FAILURE);
    }

    fdout = open(f2, O_WRONLY);
    if (fdout == -1) {
        perror(f2);
        exit(EXIT_FAILURE);
    }

    flags = fcntl(fdin, F_GETFL);
    fcntl(fdin, F_SETFL, flags & ~O_NONBLOCK);

    for (;;) {
        int res;
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(fdin, &readfds);

        res = select(fdin + 1, &readfds, NULL, NULL, NULL);
        if (res == -1) {
            perror("select");
            continue;
        }

        if (FD_ISSET(fdin, &readfds)) {
            res = transfer_partial_data(fdin, 1);

            if (res == 0) {
                printf("The partner has disconnected\n");
                break;
            }
        }
        if (FD_ISSET(0, &readfds)) {
            res = transfer_partial_data(0, fdout);
            
            if (res == 0) {
                break;
            }
        }
    }

    close(fdout);
    close(fdin);
}


int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Expected: %s <f1> <f2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    start_chat(argv[1], argv[2]);
    return 0;
}
