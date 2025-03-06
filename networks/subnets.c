#include <stdio.h>

#define NUM_IP_OCTETS 4

static int parse_ip(const char *str, unsigned *ip)
{
    char c;
    int n, i;
    unsigned parts[NUM_IP_OCTETS];

    n = sscanf(str, "%u.%u.%u.%u%c",
               parts + 3, parts + 2, parts + 1, parts, &c);
    if (n != NUM_IP_OCTETS) {
        return 0;
    }

    for (i = 0; i < NUM_IP_OCTETS; i++) {
        if (parts[i] > 255) {
            return 0;
        }
    }

    *ip = (parts[3] << 24) | (parts[2] << 16) | (parts[1] << 8) | parts[0];
    return 1;
}

static void print_subnets(unsigned ip)
{
    int i;
    unsigned mask;

    mask = ~0;
    for (i = 32; i >= 0; i--) {
        ip &= mask;
        printf("%u.%u.%u.%u/%d\n", ip >> 24, (ip >> 16) & 0xFF,
               (ip >> 8) & 0xFF, ip & 0xFF, i);
        mask <<= 1;
    }
}

int main(int argc, char **argv)
{
    int ok;
    unsigned ip;

    if (argc != 2) {
        fprintf(stderr, "Expected: ./prog <ip>\n");
        return 1;
    }

    ok = parse_ip(argv[1], &ip);
    if (!ok) {
        fprintf(stderr, "The string is not a valid ip\n");
        return 2;
    }
    print_subnets(ip);
    return 0;
}
