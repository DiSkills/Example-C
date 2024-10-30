#ifndef STR_H
#define STR_H

typedef struct {
    unsigned len,
             size;
    char *data;
} str_t;

void str_init(str_t *str);
void str_del(str_t *str);
void str_clear(str_t *str);
void str_append(str_t *str, char c);

#endif
