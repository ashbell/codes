#include    "request.h"
extern int cli_args (int argc, char **argv);
extern int send_err(int fd, int errcode, const char *msg);
extern int send_fd(int fd, int fd_to_send);


void request(char *buf, int nread, int fd)
{
    int     newfd;
    if (buf[nread-1] != 0)
    {
        sprintf(errmsg, "request not null ternimaled: %*.*s\n",
                nread, nread, buf);
        send_err(fd, -1, errmsg);
        return ;
    }
    if (buf_args(buf, cli_args) < 0)
    {
        send_err(fd, -1, errmsg);
        return ;
    }
    if ((newfd = open(pathname, oflag)) < 0)
    {
        sprintf(errmsg, "cann't open %s \n", pathname);
        return ;
    }
    if (send_fd(fd, newfd) <0)
        printf("send_fd error\n");
    close(newfd);
}



















