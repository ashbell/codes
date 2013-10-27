#include 	"tcp_clip.h"

void tcp_clip(FILE *fp, int sockfd)
{
	ssize_t 	n;
	char 		sendline[4096], recvline[4096];

	while (fgets(sendline, 4096, fp) != NULL)
	{
		write(sockfd, sendline, strlen(sendline));
		if ((n = read(sockfd, recvline, 4096)) == 0)
			printf("tcp_clip: server termilated prematurely!\n");
			exit(1);
	}
	write(1, recvline, n);
}




