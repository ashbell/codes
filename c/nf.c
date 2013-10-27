#include <stdio.h>

int main(int argc, char *argv[])
{
    int i,j;
    float result;
    for (i = 1, j = 1; i <= atoi(argv[1]); i++) {
        result = i * j;
        j = result;
    }
    printf("%f\n", result);
    return 0;



}
