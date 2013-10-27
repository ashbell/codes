#include    "apue.h"
#define MAX 4096

static void sig_pipe(int);

int main(void)
{
    int   n, fd1[2], fd2[2];
    pid_t pid;
    char line[MAX];
     
    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        printf("error when signal\n");
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        printf("error when pipe\n");
    if ((pid = fork()) < 0) 
    {
        printf("error when fork()\n");
    } 
    else if (pid > 0) 
    {
        close(fd1[0]);
        close(fd2[1]);
        while (fgets(line, MAX, stdin) != NULL) 
        {
            n = strlen(line);
            if (write(fd1[1], line, n) != n)
                printf("error when write fd1[1]\n");
            if ((n = read(fd2[0], line, MAX)) < 0)
                printf("error when read pipe fd2[0]\n");
            if (n == 0) {
                printf("child close pipe\n");
                break;
            }
            line[n] = 0;
            if (fputs(line, stdout) == EOF)
                printf("error when fputs line to stdout\n");
        }
        
        if (ferror(stdin))
            printf("fgets error stdin\n");
        exit(0);
    } else {
        close(fd1[1]);
        close(fd2[0]);
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                printf("error when dup2 fd1[0]\n");
            close(fd1[0]);
        }
     
        if (fd2[1] != STDIN_FILENO) {
            if (dup2(fd2[1], STDIN_FILENO) != STDIN_FILENO)
                printf("error when dup2 fd2[1]\n");
            close(fd2[1]);           
        }     
            
        if (execl("./add2", "", (char *)0) < 0)
            printf("error when execl add2\n");
    }

    exit(0);
}

static void sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}

/* 2011年 09月 22日 星期四 19:49:12 CST */

