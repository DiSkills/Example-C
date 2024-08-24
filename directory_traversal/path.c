#include "path.h"


static void path_append(path_t *path, const char *s)
{
    for (; *s; s++) {
        str_append(path, *s);
    }
}


void path_init(path_t *path, const char *s)
{
    str_init(path);
    path_append(path, s);
}


void path_del(path_t *path)
{
    str_del(path);
}


void path_join(path_t *path, const char *s)
{
    str_append(path, '/');
    path_append(path, s);
}


void path_unjoin(path_t *path)
{
    while (path->data[path->len] != '/') {
        path->len--;
    }
    path->data[path->len] = '\0';
}
