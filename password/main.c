#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "str.h"


static void get_str(str_t *str, FILE *file)
{
    int c;

    str_init(str);
    while ((c = fgetc(file)) != '\n') {
        str_append(str, c);
    }
}


static int str_compare(const str_t *str1, const str_t *str2)
{
    return strcmp(str1->data, str2->data);
}


int main(int argc, char **argv)
{
    FILE *file;
    str_t input, password;
    struct termios ts1, ts2;
    int password_is_corrected;

    if (argc != 2) {
        fprintf(stderr, "Expected: <filename>\n");
        exit(1);
    }
    if (!isatty(0)) {
        fprintf(stderr, "Terminal input was expected\n");
        exit(2);
    }

    file = fopen(argv[1], "r");
    if (!file) {
        perror(argv[1]);
        exit(3);
    }
    get_str(&password, file);
    fclose(file);

    tcgetattr(0, &ts1);
    memcpy(&ts2, &ts1, sizeof(ts1));
    ts1.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &ts1);

    printf("Password: ");
    get_str(&input, stdin);
    tcsetattr(0, TCSANOW, &ts2);

    password_is_corrected = (str_compare(&input, &password) == 0) ? 1 : 0;
    str_del(&password);
    str_del(&input);

    if (!password_is_corrected) {
        fprintf(stderr, "Incorrect password\n");
        exit(4);
    }
    return 0;
}
