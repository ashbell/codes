#include <stdio.h>

int func(void)
{
    static int counter = 1;
    int c;
    c = ++counter;
    printf("%d\n", c);
    return c;
}

int main(void)
{

    static int a = 20;
    a = 100;
    printf("a:%d\n", a);
    int answer;
    answer = func() - func() * func();
    printf("%d\n", answer);
    return 0;

}
