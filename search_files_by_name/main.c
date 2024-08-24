#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "path.h"


typedef void (*fcallbackp)(path_t *, const char *, void *);

static void traverse_directory(path_t *path,
        fcallbackp callback, void *userdata);


static int dir_is_itself(const char *dir)
{
    return strcmp(dir, ".") == 0;
}


static int dir_is_parent(const char *dir)
{
    return strcmp(dir, "..") == 0;
}


static void filecmp_print_callback(path_t *path, const char *name,
        void *userdata)
{
    const char *expected = userdata;
    if (strcmp(name, expected) == 0) {
        printf("%s\n", path->data);
    }
}


static void process_file(path_t *path, const char *short_name,
        fcallbackp callback, void *userdata)
{
    int res;
    struct stat sb;

    res = lstat(path->data, &sb);
    if (res == -1) {
        perror(path->data);
        return;
    }

    if ((sb.st_mode & S_IFMT) == S_IFDIR) {
        traverse_directory(path, callback, userdata);
    } else {
        callback(path, short_name, userdata);
    }
}


static void traverse_directory(path_t *path,
        fcallbackp callback, void *userdata)
{
    struct dirent *dent;
    DIR *dir = opendir(path->data);
    if (!dir) {
        perror(path->data);
        return;
    }

    while ((dent = readdir(dir)) != NULL) {
        if (dir_is_itself(dent->d_name) || dir_is_parent(dent->d_name)) {
            continue;
        }
        path_join(path, dent->d_name);
        process_file(path, dent->d_name, callback, userdata);
        path_unjoin(path);
    }

    closedir(dir);
}


int main(int argc, char **argv)
{
    path_t path;

    if (argc != 2) {
        fputs("Expected: <filename>\n", stderr);
        return 1;
    }
    path_init(&path, ".");

    traverse_directory(&path, filecmp_print_callback, argv[1]);

    path_del(&path);
    return 0;
}
