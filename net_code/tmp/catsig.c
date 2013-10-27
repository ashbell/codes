#include    <signal.h>
#include    <stdio.h>
#include    <unistd.h>

static void sig_usr(int);

int main(int argc, char *argv[])
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        printf("error catch SIGUSR1\n");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        printf("error catch SIGUSR2\n");

    for ( ; ; )
        pause();
}

static void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("recevied SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("recevied SIGUSR2\n");
    else
        printf("recevied signal %d \n", signo);
}




