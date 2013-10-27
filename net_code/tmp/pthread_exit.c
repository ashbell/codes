/* 线程栈的内容会随着线程的退出而改变 */

#include    <stdio.h>
#include    <pthread.h>
#include    <sys/types.h>
#include    <unistd.h>

struct foo {
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);
    printf(" structure at 0x%x\n", (unsigned)fp);
    printf(" foo.a = %d\n", fp->a);
    printf(" foo.b = %d\n", fp->b);
    printf(" foo.c = %d\n", fp->c);
    printf(" foo.d = %d\n", fp->d);
}

void *thr_fn1(void *arg)
{
    struct foo foo = {1, 2, 3, 4};
    printfoo("thread 1: \n", &foo);
    pthread_exit((void *) &foo);
}

void *thr_fn2(void *arg)
{
    printf("thread 2: ID is %d\n", (unsigned int)pthread_self());
    pthread_exit((void *) 0);
}

int main(void)
{
    int     err;
    pthread_t tid1, tid2;
    static struct foo *fp = NULL, pfoo;
    fp = &pfoo;
    fp->a = 12;
    fp->b = 13;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        printf("error when creat thread1\n");
    err = pthread_join(tid1, (void *) &fp);
    if (err != 0)
        printf("error when join thread1\n");
    sleep(1);
    printf("parent starting sencond thread\n");
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        printf("error when creat thread2\n");
    sleep(1);
    printfoo("parent:\n", fp);
    return 0;
}









