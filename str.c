int string_length(const char* str)
{
    const char* p;
    for (p = str; *p; p++)
        {}
    return p - str;
}

void string_copy(char* dest, const char* src)
{
    for (; *src; src++, dest++)
        *dest = *src;
    *dest = '\0';
}
