#include    "opend.h"

char errmsg[MAXLINE];
int  oflag;
char *pathname;

int main(void)
{
    int     nread;
    char    buf[MAXLINE];

    for ( ; ; )
    {
        if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
            printf("read error on stram pipe\n");
        else if (nread == 0)
            break;
        request(buf, nread, STDOUT_FILENO);
    }
    exit(0);
}






