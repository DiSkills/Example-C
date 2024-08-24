#ifndef PATH_H
#define PATH_H

#include "str.h"

typedef struct str_t path_t;

void path_join(path_t *path, const char *s);
void path_unjoin(path_t *path);

#endif
