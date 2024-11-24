#include <limits.h>

#include <arpa/inet.h>

#include "inet_port_aton.h"


static int is_digit(char c)
{
    return '0' <= c && c <= '9';
}


unsigned short inet_port_aton(const char *s)
{
    unsigned port;

    port = 0;
    for (; *s; s++) {
        if (!is_digit(*s)) {
            return 0;
        }
        port *= 10;
        port += *s - '0';
    }
    if (port > USHRT_MAX) {
        return 0;
    }
    return htons((unsigned short)port);
}
