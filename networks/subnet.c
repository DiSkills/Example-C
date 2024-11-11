#include <stdio.h>
#include <stdlib.h>

enum {
    NUM_IP_OCTETS = 4
};

typedef union {
    unsigned address;
    unsigned char octets[NUM_IP_OCTETS];
} ip_t;


static int atoip(const char *str, unsigned *pip)
{
    char c;
    int n, i;
    unsigned parts[NUM_IP_OCTETS];
    ip_t ip;

    n = sscanf(str, "%u.%u.%u.%u%c",
               parts + 3, parts + 2, parts + 1, parts, &c);
    if (n != NUM_IP_OCTETS) {
        return -1;
    }

    for (i = 0; i < NUM_IP_OCTETS; i++) {
        if (parts[i] > 255) {
            return -1;
        }
        ip.octets[i] = parts[i];
    }

    *pip = ip.address;
    return 0;
}


static void print_ip(unsigned ip_address)
{
    ip_t ip;

    ip.address = ip_address;
    printf("%u.%u.%u.%u", ip.octets[3], ip.octets[2],
           ip.octets[1], ip.octets[0]);
}


static void print_subnets(unsigned ip_address)
{
    int i;
    unsigned mask;

    mask = ~0;
    for (i = 32; i >= 0; i--) {
        ip_address &= mask;
        print_ip(ip_address);
        printf("/%d\n", i);
        mask <<= 1;
    }
}


int main(int argc, char **argv)
{
    int res;
    unsigned ip;

    if (argc != 2) {
        fprintf(stderr, "Expected: ./prog <ip>\n");
        exit(1);
    }

    res = atoip(argv[1], &ip);
    if (res == -1) {
        fprintf(stderr, "Expected ip-address\n");
        exit(2);
    }
    print_subnets(ip);
    return 0;
}
