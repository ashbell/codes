#include 	"tcp_clip.h"

int  main(int argc, char *argv[])
{
	int 			sockfd;
	struct 	sockaddr_in 	servaddr;

	if (2 != argc)
	{
		printf("useage: tcpcli <IP>\n");
		exit(1);
	}
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("error when socket\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		printf("error when connect servers!\n");

	tcp_clip(stdin, sockfd);
	exit(0);
}
