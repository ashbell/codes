#include    <stdio.h>

int main(void)
{
    //int (*arr)[4] = {{0, 1, 2, 3},{4, 56, 7, 0}};
    int tab[2][4] = { {1, 2, 3, 4}, {4, 5, 6, 7} };
    int arry[4] = {56, 6, 8, 1};
    int *pap;
    pap = tab[0]; //tab[0] 表示tab的首行地址，故不用&tab[0]
    int i, *p = &arry[0];
    p = &arry[0];
    for (i = 0; i < 4; i++)
    {
        printf("\n%d ----%d --- %d \n", *p++, arry[i], pap[i]);
    }
    return 0;

}
