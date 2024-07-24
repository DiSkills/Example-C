void remove_spaces(char *str)
{
    char *dst = str;

    for (; *str; str++) {
        if (*str != ' ') {
            *dst = *str;
            dst++;
        }
    }
    *dst = '\0';
}
