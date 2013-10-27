#include <stdio.h>
#include <stdlib.h>

int MaxSubQuence( const int A[], int N)
{
    int sum, max, j;
    sum = max = 0;
    for ( j = 0; j < N; j++)
     {
        sum += A[j];
        if (sum > max)
            max = sum;
        else if (sum < 0)
            sum = 0;
     }
     return max;
}


int main(int argc, char *argv[])
{
    int  i, A[100], v;
    i = v = 0;
    while (scanf("%d", &v) == 1)
        A[i++] = v;

    printf("max subsquence is:  %d\n", MaxSubQuence(A, i));
    i = 0;
    return 0;
}
