#include <stdio.h>

#include "calculate_expression.h"


int main(int argc, char **argv)
{
    if (argc < 2) {
        fputs("One argument was expected.\n", stderr);
        return 1;
    }
    printf("%d\n", calculate_expression(argv[1]));
    return 0;
}
