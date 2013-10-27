#include 	"tcp_serp.h"

void 	tcp_serp(int sockfd)
{
	ssize_t 		n;
	int 			randomNUM, tmp, m;
	char 			bufin[4096], *str, bufout[4096];
	
	again:
		randomNUM = random() % 10;
		while ((n =  read(sockfd, bufin, 4096)) > 0)
		{
			tmp = atoi(bufin);
			if (tmp == randomNUM)
			{
				str = "Good, you are right!\n";
				strcat(bufout, str);
			}
			else
			{
				str = "Wrong, The right number is";
				strcat(bufout, str);
				m = strlen(str);
				bufout[m] = randomNUM + '0';
				bufout[m+1] = '\n';
				bufout[m+2] = '\0';
			}

			write(sockfd, bufout, strlen(bufout));
			bzero(bufout, sizeof(bufout));
			randomNUM = random() % 10;
		}

	if (n < 0 && errno == EINTR)
			goto again;
	else if (n < 0)
	{
		printf("tcp_serp: read error!\n");
		free(str);
		exit(1);
	}
}




