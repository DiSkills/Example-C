#include <stdio.h>
#include <stdlib.h>

#include "string.h"


#define MAX(A, B) ((A) > (B) ? (A) : (B))


static struct string *max_string_from_file(const char *filename)
{
    int c;
    FILE *f;
    struct string *str,
                  *max_str = string_init();

    f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        return max_str;
    }

    str = string_init();
    while ((c = fgetc(f)) != EOF) {
        if (c != '\n') {
            string_append(c, str);
        } else {
            if (max_str->size < str->size) {
                struct string *tmp = max_str;
                max_str = str;
                str = tmp;
            }
            string_del(&str);
            str = string_init();
        }
    }
    string_del(&str);

    fclose(f);
    return max_str;
}


int main(int argc, char **argv)
{
    int i,
        max;
    struct string **strings;

    if (argc < 2) {
        fprintf(stderr, "No parameter given\n");
        return 1;
    }
    argc--;
    argv++;

    strings = malloc(argc * sizeof(struct string*));

    max = 0;
    for (i = 0; i < argc; i++) {
        strings[i] = max_string_from_file(argv[i]);
        max = MAX(max, strings[i]->size);
    }

    for (i = 0; i < argc; i++) {
        if (max == strings[i]->size) {
            putchar('*');
        }
        printf("%s:%s\n", argv[i], strings[i]->symbols);
    }

    for (i = 0; i < argc; i++) {
        string_del(&strings[i]);
    }
    free(strings);
    return 0;
}
