#include <stdio.h>

#include "string.h"


int main()
{
    int c;
    struct string *str = string_init();

    while ((c = getchar()) != EOF) {
        string_append(c, str);
    }

    string_print(str);

    string_del(&str);
    return 0;
}
