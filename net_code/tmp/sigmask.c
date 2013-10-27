#include    <stdio.h>
#include    <unistd.h>
#include    <signal.h>

static void sig_quit(int);

int main(int argc, char *argv[])
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        printf("\nerror when singal sig_quit()");

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        printf("\nerror when sigprocmask!");
    
    sleep(5);

    if (sigpending(&pendmask) < 0)
        printf("\nerror when sigpending newmask!");
    if (sigismember(&pendmask, SIGQUIT))
        printf("\nSIGQUIT is member of pendmask");
   

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        printf("\nerror when reset mask!");
    printf("\nsingmask unblocked ");
    
    if (sigpending(&pendmask) < 0)
        printf("\nerror when sigpending newmask!");
    if (sigismember(&pendmask, SIGQUIT) == 0)
        printf("\nSIGQUIT is not member of pendmask\n");

    sleep(5);
    return 0;

}

static void sig_quit(int signo)
{
    printf("\ncaught SIGQUIT signal");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        printf("\n error when signal SIG_DFL");


}




