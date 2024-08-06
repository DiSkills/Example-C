#ifndef CALLS_H
#define CALLS_H

enum opening_modes {
    O_RDONLY = 0x000,
    O_WRONLY = 0x001,
    O_CREAT = 0x040,
    O_TRUNC = 0x200
};

int open(const char *path, int mode, int rights);
int close(int fd);
int read(int fd, void *buf, int size);
int write(int fd, const void *buf, int size);
extern int errno;

#endif
