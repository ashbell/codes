#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <sys/types.h>

pthread_mutex_t  lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  lock2 = PTHREAD_MUTEX_INITIALIZER;

void  prepare(void)
{
    printf("preparing locks...\n");
    pthread_mutex_lock(&lock1);
    pthread_mutex_lock(&lock2);
}

void  parent(void)
{
    printf("parent unlocking locks...\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
}

void  chlid(void)
{
    printf("chlid unlocking locks...\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
}

void  * thr_fn(void *arg)
{
    printf("thread started....\n");
    pause();
    return (0);
}

int  main(void)
{
    int         err;
    pid_t       pid;
    pthread_t    tid;

    if ((err = pthread_atfork(prepare, parent, chlid)) != 0 )
        printf("error install fork handles!\n");
    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0)
        printf("error when creat pthread!\n");
    sleep(2);
    printf("parent about to fork ...\n");
    if ((pid = fork()) < 0)
        printf("error when fork()\n");
    else if (pid == 0)
        printf("child returned from fork\n");
    else
        printf("parent returned from fork\n");
    return 0;
}




































