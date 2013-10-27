#include    "csopen.h"

int s_pipe(int fd[2])
{
    return (pipe(fd));
}

int csopen(char *name, int oflag)
{
    pid_t           pid;
    int             len;
    char            buf[10];
    struct  iovec   iov[3];
    static  int     fd[2] = { -1, -1 };
    
    if (fd[0] < 0)
    {
        if (s_pipe(fd) < 0)
            printf("s_pipe() error\n");
        if ((pid = fork()) < 0) {
            printf("fork() error\n");
        } else {
            close(fd[0]);
            if (fd[1] != STDIN_FILENO && dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)
                printf("dup2 stdin_fileno error\n");
            if (fd[1] != STDOUT_FILENO && dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
                printf("dup2 stdout_fileno error\n");
            if (execl("./opend", "opend", (char *)0, (char *)0) < 0) /* no warning, 4 arguments! */
                printf("execl error\n");
        }
        close(fd[1]);
    }
    sprintf(buf, " %d", oflag);
    iov[0].iov_base  = CL_OPEN;
    iov[0].iov_len   = strlen(CL_OPEN);
    iov[1].iov_base  = name;
    iov[1].iov_len   = strlen(name);
    iov[2].iov_base  = buf;
    iov[2].iov_len   = strlen(buf) + 1;
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
    if (writev(fd[0], &iov[0], 3) != len)
        printf("writev error\n");
    
    return (recv_fd(fd[0], write));
}




















