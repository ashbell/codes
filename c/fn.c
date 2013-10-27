#include <stdio.h>

int fn(int n)
{
    if (0 == n)
        return 0;
    if (1 == n)
        return 1;
    else
        return fn(n-2) + fn(n-1);
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%d\n", fn(n));
    return 0;

}
