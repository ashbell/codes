#include    <stdio.h>
#include    <unistd.h>
#include    <signal.h>

static void sig_pro(int);

int main(int argc, char *argv[])
{
    int secs;
    if (signal(SIGALRM, sig_pro) == SIG_ERR)
        printf("\nerror when cat SIGALARM");
    for ( ; ; )
    {
        secs = alarm(5);
        printf("\n%d", secs);
        pause();
    }
}

static void sig_pro(int signo)
{
    if (signo == SIGALRM)
        printf("\ncat SIGALRM signal");




}
