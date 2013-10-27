/* 2011年 09月 04日 星期日 04:48:15 CST */

#include 	"open_conn.h"

static int 	count_pth = 0;
char   *hostname = "127.0.0.1";
int 	port = 0;
void  *pthread_scan(void *arg)
{
	int 	clientfd;
	struct  hostent 	*hp;
	struct 	sockaddr_in dstaddr;
	
	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket() error, exit!\n");
		exit(0);
	}
	if ((hp = gethostbyname(hostname)) == NULL)
	{
		printf("gethostname error, exit!\n");
		exit(0);
	}
	bzero((char *) &dstaddr, sizeof(dstaddr));
	dstaddr.sin_family = AF_INET;
	bcopy((const char *)hp->h_addr,
		  (char *)&dstaddr.sin_addr.s_addr, hp->h_length);
	dstaddr.sin_port = htons(port++);
	
	printf("pthread %d  scanning port: %d \n", count_pth++, port);
	if (connect(clientfd, (SA *) &dstaddr, sizeof(dstaddr)) < 0)
	{
		printf("port: %d closed!\n", port);
	}
	else
	{
		printf("pthread %d  scanning port: %d \n", count_pth++, port);
		printf("find port: %d on host: %s open!\n", port, hostname);
	}
	close(clientfd);
	pthread_exit((void *) 0);
}

int main(int argc, char *argv[])
{
	int 	port_start = 0;
	int 	port_end   = 65536;
	int 	err;
	void    *tret;
	pthread_t  tid;

	for ( ; port_start < port_end; port_start++)
	{
		err = pthread_create(&tid, NULL, pthread_scan, NULL);
		if (err != 0)
		{
			printf("error when creat pthread\n");
		}
		err = pthread_join(tid, &tret);
	}
	return 0;
}



