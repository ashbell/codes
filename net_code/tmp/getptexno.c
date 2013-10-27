/* 2011年 08月 16日 星期二 20:57:00 CST */

#include    <stdio.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <sys/types.h>

void    *thr_fn1(void *arg)
{
    printf("thread 1 running \n");
    return((void *)1);
}

void    *thr_fn2(void *arg)
{
    printf("thread 2 exiting \n");
    pthread_exit((void *)2);
}

int main(void)
{
    int     err;
    pthread_t     tid1, tid2;
    void    *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        printf("error when create pthread1\n");
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        printf("error when create pthread2\n");
    err = pthread_join(tid1, &tret);
    if (err != 0)
        printf("error when join pthread1\n");
    printf("thread 1 exit code %d \n", (int) tret);
    err = pthread_join(tid2, &tret);
    if (err != 0 )
        printf("error when join pthread2\n");
    printf("thread 2 exit code %d \n", (int) tret);
    return 0;
            
}

