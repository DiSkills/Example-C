#include <stdio.h>

#define ARRAY_SIZE(ITEM) (sizeof(ITEM) / sizeof(*(ITEM)))

#define MAKE_ARRAY_SUM_FUNCTION(FUNNAME, TYPE) \
    static TYPE FUNNAME(const TYPE *a, int n) \
    { \
        TYPE s = 0; \
        for (; n > 0; s += *a, a++, n--) \
            {} \
        return s; \
    }

MAKE_ARRAY_SUM_FUNCTION(int_array_sum, int)

MAKE_ARRAY_SUM_FUNCTION(double_array_sum, double)

int main()
{
    int isum,
        iarray[] = { 1, 2, 3, 4, 5 };
    double dsum,
           darray[] = { -1.5, 1.0, 3.0, 5.0 };

    isum = int_array_sum(iarray, ARRAY_SIZE(iarray));
    dsum = double_array_sum(darray, ARRAY_SIZE(darray));

    printf("%d %f\n", isum, dsum);
    return 0;
}
