#include    <stdio.h>
#include    <unistd.h>
#include    <pwd.h>
#include    <signal.h>
#include    <string.h>


static void my_alarm(int signo)
{
    struct passwd   *rootptr;

    printf("in signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL)
        printf("\nerror getpwnam(root");
    alarm(1);
}

int main(int argc, char *argv[])
{
    struct passwd   *ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);
    for ( ; ; )
    {
        if ((ptr = getpwnam("sar")) == NULL)
            printf("\n getpwnam error");
        if (strcmp(ptr->pw_name, "sar") != 0)
            printf("return value corrupted, pw_name = %s\n", ptr->pw_name);
        pause(); /* what the fuck happen add pasue() function */
    }
    return 0;
}




