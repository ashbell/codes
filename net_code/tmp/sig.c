#include    <stdio.h>
#include    <signal.h>
#include    <unistd.h>

static void int_sig(int);

int main(int argc, char *argv[])
{
    if (signal(SIGINT, int_sig) == SIG_ERR)
        printf("error cat SIGIN\n");
    if (signal(SIGQUIT, int_sig) == SIG_ERR)
        printf("error cat SIGQUIT\n");

    for ( ; ; )
        pause();
}

static void int_sig(int signo)
{
    if (signo == SIGINT)
        printf("\nyou may take ctrl+c\n");

    if (signo == SIGQUIT)
        printf("\nyou may quit\n");
        
}
