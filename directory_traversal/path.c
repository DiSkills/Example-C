#include "path.h"


void path_join(path_t *path, const char *s)
{
    str_append(path, '/');
    for (; *s; s++) {
        str_append(path, *s);
    }
}


void path_unjoin(path_t *path)
{
    while (path->data[path->len] != '/') {
        path->len--;
    }
    path->data[path->len] = '\0';
}
