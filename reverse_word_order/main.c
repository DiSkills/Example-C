#include <stdio.h>

#include "str_list.h"


static int is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\n';
}


int main()
{
    int c,
        last = ' ';
    struct str_list *line = str_list_init();

    while ((c = getchar()) != EOF) {
        if (!is_space(c)) {
            if (is_space(last)) {
                str_list_push_front(line);
            }
            string_append(c, line->head->str);
        }
        
        if (c == '\n') {
            str_list_print(' ', line);
            str_list_del(&line);
            line = str_list_init();
        }

        last = c;
    }

    str_list_print(' ', line);
    str_list_del(&line);
    return 0;
}
