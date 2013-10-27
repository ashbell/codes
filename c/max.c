#include <stdio.h>

int main(void)
{
    int aval[9]=  {9, -12, 120, 8, -20, 100, 30, -89, 20};
    int i, total = 0, max = 0;
    for (i = 0; i < 9; i++)
      {
        total = total + aval[i];
        if (total < 0)
            total = 0;
        max = (total >= max ? total : max);
        printf("%d :: %d \n", total, aval[i]); 
      }
    printf ("max is %d\n", max);
    return 0;
}




