#include    <stdio.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <pthread.h>

static int CountPthr = 0;
void *pth_fn(void *arg)
{
   CountPthr++;
   printf("pthread %d creat\n", CountPthr);
   printf("I'm your slave, I am doing sth for you, My Lord\n");
   sleep(1);
   printf("pthread %d exiting\n", CountPthr);
   pthread_exit((void *) 0);
}

int main(void)
{
    int     err;         
    pthread_t   tid;
    int     i;
    void    *tret;
    for (i = 0; i < 10; i++)
    {
        err = pthread_create(&tid, NULL, pth_fn, NULL);
        if (err != 0)
            printf("error when creat thread\n");
        err = pthread_join(tid, &tret);
        if (err != 0)
            printf("Oh sorry man, can't exit pthread normally\n");
        sleep(2);
    }
    printf("I finished all work, do you wanna fuck me??\n");
    return 0;
}





