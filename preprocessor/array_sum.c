#include <stdio.h>


#define ARRAY_SIZE(ITEM) (sizeof(ITEM) / sizeof(*(ITEM)))

#define MAKE_ARRAY_SUM_FUNCTION(TYPE)                                        \
    static TYPE TYPE ## _array_sum(const TYPE *a, int n)                     \
    {                                                                        \
        int i;                                                               \
        TYPE s = 0;                                                          \
        for (i = 0; i < n; i++) {                                            \
            s += a[i];                                                       \
        }                                                                    \
        return s;                                                            \
    }

MAKE_ARRAY_SUM_FUNCTION(int);
MAKE_ARRAY_SUM_FUNCTION(double);

int main()
{
    int int_sum,
        int_array[] = { 1, 2, 3, 4, 5 };
    double double_sum,
           double_array[] = { -1.5, 1.0, 3.0, 5.0 };

    int_sum = int_array_sum(int_array, ARRAY_SIZE(int_array));
    double_sum = double_array_sum(double_array, ARRAY_SIZE(double_array));

    printf("%d %f\n", int_sum, double_sum);
    return 0;
}
