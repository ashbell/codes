#include    "games.h"

void tcp_serp(int );

int     main(int argc, char *argv[])
{
    int             listenfd, connfd;
    pid_t           childpid;
    socklen_t       clilen;
    struct sockaddr_in cliaddr, servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("error when socket listenfd\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9877);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        printf("error when bind sockaddr!\n");
    listen(listenfd, 1024);

    for (; ;)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0)
            printf("error when accept listen!\n");
        if ((childpid = fork()) == 0)
        {
            close(listenfd);
            tcp_serp(connfd);
            exit(0);
        }
        close(connfd);
    }
    
    return 0;
}



