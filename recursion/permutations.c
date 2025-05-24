#include <stdio.h>

#define SWAP(A, B)                                                           \
    do {                                                                     \
        int tmp = (A);                                                       \
        (A) = (B);                                                           \
        (B) = tmp;                                                           \
    } while (0)

static void print_numbers(int *arr, int length)
{
    int i;
    for (i = 0; i < length; i++) {
        printf("%d%c", arr[i], i == length - 1 ? '\n' : ' ');
    }
}

static void permutations_do(int *arr, int start, int length)
{
    int i;
    if (length - start == 1) {
        print_numbers(arr, length);
        return;
    }
    for (i = start; i < length; i++) {
        SWAP(arr[start], arr[i]);
        permutations_do(arr, start + 1, length);
        SWAP(arr[start], arr[i]);
    }
}

static void permutations(int *arr, int length)
{
    permutations_do(arr, 0, length);
}

int main()
{
    int arr[] = { 1, 2, 3 };
    permutations(arr, sizeof(arr) / sizeof(*arr));
    return 0;
}
