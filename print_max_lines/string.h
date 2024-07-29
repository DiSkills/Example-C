#ifndef STRING_H
#define STRING_H

struct string {
    unsigned size,
             max_size;

    unsigned char *symbols;
};

struct string *string_init();

void string_del(struct string **pstr);

void string_append(unsigned char ch, struct string *str);

#endif
