#include <stdio.h>

static void intvec2max_not_tail(const int *arr, int len, int res[2])
{
    if (len == 2) {
        if (arr[0] > arr[1]) {
            res[0] = arr[0];
            res[1] = arr[1];
        } else {
            res[0] = arr[1];
            res[1] = arr[0];
        }
        return;
    }

    intvec2max_not_tail(arr + 1, len - 1, res);
    if (*arr > res[0]) {
        res[1] = res[0];
        res[0] = *arr;
    } else if (*arr > res[1]) {
        res[1] = *arr;
    }
}

static void intvec2max_do(const int *arr, int len, int res[2])
{
    if (len == 0) {
        return;
    }
    if (*arr > res[0]) {
        res[1] = res[0];
        res[0] = *arr;
    } else if (*arr > res[1]) {
        res[1] = *arr;
    }
    intvec2max_do(arr + 1, len - 1, res);
}

static void intvec2max(const int *arr, int len, int res[2])
{ /* tail recursion & accumulator */
    if (arr[0] > arr[1]) {
        res[0] = arr[0];
        res[1] = arr[1];
    } else {
        res[0] = arr[1];
        res[1] = arr[0];
    }
    intvec2max_do(arr + 2, len - 2, res);
}

int main()
{
    int arr[] = { 2, 1, -2, -5, 5, 0, 1, 2, 3, 4 };
    int res[2];
    intvec2max(arr, sizeof(arr) / sizeof(*arr), res);
    printf("%d %d\n", res[0], res[1]);
    return 0;
}
