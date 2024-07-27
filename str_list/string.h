#ifndef STRING_H
#define STRING_H

struct char_item {
    unsigned char data;

    struct char_item *next;
};

struct string {
    unsigned size;

    struct char_item *head,
                     *tail;
};

struct string *string_init();

void string_append(unsigned char data, struct string *str);

void string_print(const struct string *str);

void string_del(struct string **pstr);

#endif
