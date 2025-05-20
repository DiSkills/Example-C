#include <stdio.h>

#define LENGTH(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))

static int reset_max_do(int *arr, int len, int max)
{
    if (len == 0) {
        return max;
    }
    max = reset_max_do(arr + 1, len - 1, *arr > max ? *arr : max);
    if (*arr == max) {
        *arr = 0;
    }
    return max;
}

static void reset_max(int *arr, int len)
{
    if (len == 0) {
        return;
    }
    reset_max_do(arr + 1, len - 1, *arr);
}

int main()
{
    int i;
    int arr[] = { 8, 5, 4, 8, 2, 3, 1, 8, 7, 9, 8, 9, 5, 4, 9, 2, 1 };

    reset_max(arr, LENGTH(arr));
    for (i = 0; i < LENGTH(arr); i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');
    return 0;
}
