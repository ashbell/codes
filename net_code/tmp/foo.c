#include    <stdio.h>

int main(void)
{
    struct foo {
        int a;
        int b;
    };
    struct foo foo, *pfoo = NULL, tmp;
    foo.a = 1;
    foo.b = 2;
    printf("struct foo foo: %d %d\n", foo.a, foo.b);
   
    pfoo = &tmp;
    pfoo->a = 3;
    pfoo->b = 4;
    printf("struct foo *pfoo: %d %d\n", pfoo->a, pfoo->b);
    return 0;

}
