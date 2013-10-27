#define MAX 1024
#include    "apue.h"

int main(void)
{
    int     n;
    int     fd[2];
    pid_t   pid;
    char    line[MAX];

    if (pipe(fd) < 0)
        printf("pipe error!\n");
    if ((pid = fork()) < 0) {
        printf("fork error!\n");
    } else if (pid > 0) {
        close(fd[0]);
        write(fd[1], "Hello I come from parent!\n", 26);
    } else {
        close(fd[1]);
        n = read(fd[0], line, MAX);
        write(STDOUT_FILENO, line, n);
    }
    return 0;

}






















