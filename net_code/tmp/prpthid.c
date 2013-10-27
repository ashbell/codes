#include    <stdio.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <sys/types.h>

pthread_t   ntid;
void printids(const char *s)
{
    pid_t   pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u  tid %u (0x%x)\n", s, (unsigned int ) pid,
            (unsigned int)tid, (unsigned int )tid); 
}

void *thr_fn(void *arg)
{
    printids("this is the fucker new thread: ");

    return ((void *)0);
}

int main(void)
{
    int     err;
    err = pthread_create(&ntid, NULL,thr_fn, NULL);
    if (err != 0)
        printf("can't creat thread:  \n");
    printids("main thread:");
    sleep(1);
    return 0;


}







