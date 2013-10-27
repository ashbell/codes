#include    <stdio.h>

struct foo {
    int a;
    int b;
};

struct foo  *change(const struct foo *src)
{
    struct foo fuck;
    struct foo *foo = &fuck;
    foo->a = src->a * src->a;
    foo->b = src->b + src->b;
    return foo;
}

int main(void)
{
    struct foo src, suck;
    struct foo *tmp = &suck;
    src.a = 3;
    src.b = 5;
    tmp = change(&src);
    printf("%d -- %d\n", tmp->a, tmp->b);
    return 0;

}
