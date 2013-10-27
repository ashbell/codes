#include    <stdio.h>
//有没有什么办法能让我定义一个指针*p，p能使用a的值，
//然后我修改p的值却又不影响a的值呢？ p和a指向的是同
//一个内存区域，更改p必然导致a值的变化～！

int sum(int *a, int n)
{
    int i;
    int sum = 0;
    int p[4];
    p[0] = a[0];
    p[1] = a[1];
    p[2] = a[2];
    p[3] = a[3];


    for (i = 0; i < n; i++)
    {
        p[i] = p[i] + 2;
        sum = sum + p[i];
        printf("%d\n", p[i]);
    }
    return sum;
}

int main(void)
{
    int arr[4] = {1, 2, 3, 4};
    int p = sum(&arr[0], 4);
    printf("%d -- %d  -- %d -- %d -- %d\n", p, arr[0], arr[1],arr[2], arr[3]);
    return 0;
}
