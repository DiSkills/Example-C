#ifndef STR_H
#define STR_H

struct str {
    unsigned size,
             max_size;
    char *items;
};

void str_init(struct str *s);
void str_del(struct str *s);
void str_append(char ch, struct str *s);

#endif
