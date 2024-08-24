#include <stdio.h>
#include <sys/stat.h>
#include <time.h>


const char *get_file_type(int mode)
{
    switch (mode & S_IFMT) {
        case S_IFBLK:
            return "block device";
        case S_IFCHR:
            return "character device";
        case S_IFDIR:
            return "directory";
        case S_IFIFO:
            return "FIFO/pipe";
        case S_IFLNK:
            return "symlink";
        case S_IFREG:
            return "regular file";
        case S_IFSOCK:
            return "socket";
    }
    return "unknown?";
}


void print_stat(const struct stat *sb)
{
    printf("ID of containing device: %lu\n", sb->st_dev);
    printf("I-node number: %lu\n", sb->st_ino);

    printf("File type: %s\n", get_file_type(sb->st_mode));
    printf("Mode: %o\n", sb->st_mode & 07777);
    printf("Link count: %lu\n", sb->st_nlink);

    printf("Uid: %u\n", sb->st_uid);
    printf("Gid: %u\n", sb->st_gid);

    printf("File size: %lu bytes\n", sb->st_size);
    printf("Preferred I/O block size: %ld bytes\n", sb->st_blksize);
    printf("Number of blocks: %ld\n", sb->st_blocks);

    printf("Last file access: %s", ctime(&sb->st_atime));
    printf("Last file modification: %s", ctime(&sb->st_mtime));
    printf("Last status change: %s", ctime(&sb->st_ctime));
}


int print_file_stat(const char *filename)
{
    int res;
    struct stat sb;

    res = lstat(filename, &sb);
    if (res == -1) {
        perror(filename);
        return -1;
    }
    print_stat(&sb);
    if ((sb.st_mode & S_IFMT) != S_IFLNK) {
        return 0;
    }

    printf("\nRefers to:\n");
    res = stat(filename, &sb);
    if (res == -1) {
        printf("Dangling\n");
    } else {
        print_stat(&sb);
    }
    return 0;
}


int main(int argc, char **argv)
{
    int res;

    if (argc != 2) {
        fputs("Expected: <filename>\n", stderr);
        return 1;
    }

    res = print_file_stat(argv[1]);
    if (res == -1) {
        return 2;
    }

    return 0;
}
