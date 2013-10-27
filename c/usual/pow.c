#include    <stdio.h>

int powc(int base, int n)
{
    int pa = 1;
    for (; n > 0; --n)
        pa = pa * base;
    return pa;

}

int main(void)
{
    int n = 3;
    int ba = 10;
    int p = powc(ba, 3);
    printf("%d -- %d\n", p, n);
    return 0;

}
