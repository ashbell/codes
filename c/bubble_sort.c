#include <stdio.h>

void bubble_sort(int *arr, int sz)
{
    int i, j, t;
    for (i = 0; i < (sz - 1); i++) {
      for (j = i + 1; j < sz; j++) {
        if (arr[i] > arr[j]) {
          t = arr[i];
          arr[i] = arr[j];
          arr[j]  = t;
        }
     }
   }
}

int main(void)
{
    int a[] = {10, 28, 3, 56, 22, 67, 0, -1, 30, 25, 7, 15};
    int i = 0;
    bubble_sort (a, sizeof(a)/sizeof(int));
    printf ("%d -- %d -- %d\n", sizeof(a), sizeof(int), sizeof(a)/sizeof(int));
    while (i < sizeof(a)/sizeof(int))
      printf ("%d  ", a[i++]);
    printf ("\n");
    return 0;
}







