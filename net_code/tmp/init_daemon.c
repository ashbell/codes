#include    <sys/times.h>
#include    <limits.h>
#include    <signal.h>
#include    <sys/types.h>
#include    <stdio.h>
#include    <unistd.h>
#include    <syslog.h>
#include    <fcntl.h>
#include    <sys/resource.h>

void    deamonize(const char *cmd)
{
    int         i, fd0, fd1, fd2;
    pid_t       pid;
    struct sigaction    sa;
    struct rlimit      rl;
    
    umask(0);
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        printf("error when getlimit!\n");

    if ((pid = fork()) < 0)
        printf("error when fork()_\n");
    else if (pid != 0)
        printf("error");
    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        printf(" cannot fork()!\n");
    if ((pid = fork()) < 0)
        printf("cant fork\n");
    else if (pid != 0)
        printf("error");

    if (chdir("/") < 0)
        printf("can't change directory\n");

    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpect file descriptiors %d %d %d", fd0, fd1, fd2);
        printf("error");
    }

}


int main(void)
{
    deamonize("Fuck you");
    sleep(30);
    return 0;

}




